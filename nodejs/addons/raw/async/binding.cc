#include "node.h"
#include "v8.h"
#include "uv.h"

#include <assert.h>
#include <vector>
#include <memory>

namespace {

  // 运行回调函数
void RunInCallbackScope(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  // 参数校验
  assert(args.Length() == 4);
  assert(args[0]->IsObject());
  assert(args[1]->IsNumber());
  assert(args[2]->IsNumber());
  assert(args[3]->IsFunction());

  // 构造异步资源的唯一标识符和触发当前异步资源的父异步资源的标识符
  // struct async_context {
  //   double async_id;    // 当前异步资源的唯一标识符
  //   double trigger_async_id; // 触发当前异步资源的父异步资源的标识符
  // };
  node::async_context asyncContext = {
    args[1].As<v8::Number>()->Value(),
    args[2].As<v8::Number>()->Value()
  };
  // 构造回调函数的上下文
  node::CallbackScope scope(isolate, args[0].As<v8::Object>(), asyncContext);

  // 获取回调函数
  v8::Local<v8::Function> fn = args[3].As<v8::Function>();

  // 调用回调函数 call第二个参数指定回调函数this（若箭头函数则该this失效），第三个参数指定回调函数参数个数，第四个参数指定回调函数参数
  // V8_WARN_UNUSED_RESULT MaybeLocal<Value> Call(
  //   Local<Context> context,       // 1. 执行上下文
  //   Local<Value> recv,           // 2. 函数调用中的 'this' 值
  //   int argc,                    // 3. 实参个数
  //   Local<Value> argv[]          // 4. 实参值数组
  // )
  v8::Local<v8::Value> argv[] = { args[0] };
  v8::MaybeLocal<v8::Value> ret = fn->Call(isolate->GetCurrentContext(), v8::Undefined(isolate), std::size(argv), argv);

  if (!ret.IsEmpty())
    args.GetReturnValue().Set(ret.ToLocalChecked());
}

// 异步的cpp回调函数
// ignored: 在 uv_queue_work 的完成回调中，这个参数通常被忽略（旧版 libuv 可能用于错误码，但 Node.js 抽象层通常用其他方式处理）
static void Callback(uv_work_t* req, int ignored) {
  // 获取当前 V8 隔离实例 (Isolate)，这是与 V8 引擎交互的必要句柄
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  // HandleScope: 创建一个句柄作用域。在作用域内创建的所有 Local 句柄（如 local）在作用域结束时会自动被清理，防止内存泄漏
  v8::HandleScope scope(isolate);
  
  // node::CallbackScope: 这是 Node.js 特有的，用于支持 async_hooks。
  //   它通知 Node.js 的异步钩子系统（async_hooks）：一个异步回调即将开始执行。
  //   这使得 async_hooks 能够正确地追踪异步调用链、执行 before/after 钩子、以及实现 AsyncLocalStorage (ALS) 的上下文传递。
  //   v8::Object::New(isolate) 创建了一个临时对象作为 recv (this 值)，这里只是为了满足参数要求，实际意义不大。
  //   node::async_context{0, 0} 提供了 asyncId 和 triggerAsyncId。{0, 0} 是一个占位符，表示不关联特定的异步资源 ID。在实际生产代码中，通常会传递从 TestResolveAsync 传入的有效 async_context 以保持调用链完整
  node::CallbackScope callback_scope(isolate, v8::Object::New(isolate), node::async_context{0, 0});
  
  // 获取并管理资源: 从 req->data 中取出之前存储的 persistent 指针。
  //   std::unique_ptr: 使用智能指针接管这个 new 出来的指针。unique_ptr 的析构会自动 delete 它管理的对象。这确保了 persistent 指针本身在 Callback 结束后被正确释放
  std::unique_ptr<v8::Global<v8::Promise::Resolver>> persistent { static_cast<v8::Global<v8::Promise::Resolver>*>(req->data) };
  
  // 解析 Promise: 这是异步操作的最终结果。
  //   从 Global 句柄获取 Local 句柄。
  //   调用 local->Resolve(...)，将 Promise 的状态从 "pending" 变为 "fulfilled"。
  //   解析值为 v8::Undefined(isolate)，对应 JavaScript 的 undefined。
  //   .ToChecked() 忽略了 Maybe 类型的检查（假设 Resolve 不会失败，但实际代码中应检查 IsEmpty()）
  v8::Local<v8::Promise::Resolver> local = persistent->Get(isolate);
  // local->Resolve(isolate->GetCurrentContext(), v8::Undefined(isolate)).ToChecked();
  local->Resolve(isolate->GetCurrentContext(), v8::Number::New(isolate, 123)).ToChecked();
  delete req;
}

// cpp构建异步函数
static void TestResolveAsync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  // 获取当前 V8 隔离实例 (Isolate)，这是与 V8 引擎交互的必要句柄
  v8::Isolate* isolate = args.GetIsolate();

  // 创建 Promise::Resolver: 调用 v8::Promise::Resolver::New(context) 创建一个新的 Promise 和与之关联的 Resolver 对象。ToLocalChecked() 将 MaybeLocal 转换为 Local。
  // 使用 Global: v8::Global 是一个持久化句柄（Persistent Handle）。它确保 Promise::Resolver 对象在 V8 的垃圾回收中不会被销毁，即使它暂时没有被 JavaScript 代码引用。这对于跨异步操作（从 JS 调用到 C++ 回调）保持对象存活至关重要。
  // 动态分配 (new): persistent 指针被 new 分配在堆上。它的生命周期将由 uv_work_t 的 data 字段和最终的 Callback 函数管理
  v8::Global<v8::Promise::Resolver>* persistent = new v8::Global<v8::Promise::Resolver>( 
    isolate, 
    v8::Promise::Resolver::New( isolate->GetCurrentContext()).ToLocalChecked()
  );

  // 创建 uv_work_t: 这是 libuv 用于异步工作请求的结构体
  uv_work_t* req = new uv_work_t;
  // 将指向 persistent 的指针存储在 req->data 中。这是在异步工作完成时（在 Callback 中）找回 Promise::Resolver 的唯一方式
  req->data = static_cast<void*>(persistent);

  // uv_queue_work 将一个任务排入 libuv 的工作线程池。
  uv_queue_work(
    // node::GetCurrentEventLoop(isolate): 获取与当前 Isolate 关联的 libuv 事件循环。
    node::GetCurrentEventLoop(isolate),
    // req: 工作请求。
    req,
    // [](uv_work_t*) {}: 工作回调 (Work Callback)。这个 lambda 函数在 libuv 的工作线程池中的一个线程上执行。在这个例子中，它是空的 {}，意味着没有实际的耗时工作要做。这通常用于模拟一个立即完成的异步操作，或者实际工作在 Callback 中完成（虽然不推荐，因为 Callback 应在事件循环线程执行）。
    [](uv_work_t*) {},
    // Callback: 完成回调 (After Work Callback)。当工作回调（在工作线程）执行完毕后，libuv 会自动调度此回调在**主线程（事件循环线程）**上执行。这是我们与 V8 交互（操作 JavaScript 对象）的安全位置
    Callback
  );
  // CPU 密集型任务，将计算分流到线程池，避免阻塞主线程，永远不在工作线程访问 V8 API，libuv 工作函数中只能操作非 V8 数据
    // uv_queue_work(..., [](uv_work_t* req) {
    //   // 在子线程执行复杂计算
    //   auto data = static_cast<ComputeData*>(req->data);
    //   data->result = HeavyComputation();
    // }, Callback);


  // 获取 Promise 对象: 从 Global 句柄中获取 Local 句柄。
  v8::Local<v8::Promise::Resolver> local = persistent->Get(isolate);
  // 返回 Promise: 调用 local->GetPromise() 获取与 Resolver 关联的 Promise 对象，并通过 args.GetReturnValue().Set(...) 将其返回给 JavaScript 调用者。
  args.GetReturnValue().Set(local->GetPromise());
  // 关键点: TestResolveAsync 立即返回。它不等待异步操作完成。JavaScript 代码拿到的是一个尚未解析的 Promise。稍后，当 Callback 执行时，这个 Promise 才会被解析
}

void Initialize(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "runInCallbackScope", RunInCallbackScope);
  NODE_SET_METHOD(exports, "testResolveAsync", TestResolveAsync);
}

}  // anonymous namespace

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)