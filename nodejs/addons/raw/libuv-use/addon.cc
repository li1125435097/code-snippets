#include <node.h>
#include <v8.h>
#include <uv.h>

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Value;
using v8::Int32;

// 前向声明 TimerCallback
void TimerCallback(uv_timer_t* handle);

// 定义 TimerCallbackData 结构体
// libuv 的 C 回调（uv_timer_cb）无法直接调用 V8 的 v8::Function。我们必须在 C++ 层设置一个中间层，将 C 回调转换为 V8 环境下的函数调用
struct TimerCallbackData {
  Isolate* isolate;
  v8::Global<v8::Function> callback;
  void* original_data;

  TimerCallbackData(Isolate* iso, Local<Function> func, void* orig_data)
      : isolate(iso), original_data(orig_data) {
    // 建立一个从 C++ 代码到 V8 堆上 JavaScript 对象（在这个例子中是 func 函数）的持久引用，防止该对象被 V8 的垃圾回收器 (Garbage Collector, GC) 错误地回收
    callback.Reset(isolate, func);
  }

  ~TimerCallbackData() {
    // 清理持久引用
    callback.Reset();
  }
};

// 定义真正的 CloseCallback (带清理)
void CloseCallback(uv_handle_t* handle) {
  uv_timer_t* timer = reinterpret_cast<uv_timer_t*>(handle);
  TimerCallbackData* cb_data = static_cast<TimerCallbackData*>(timer->data);
  if (cb_data != nullptr) {
    delete cb_data; // 清理 TimerCallbackData 和 Global<v8::Function>
    timer->data = nullptr;
  }
  delete timer; // 清理 uv_timer_t 本身
}

// 实现 TimerCallback
void TimerCallback(uv_timer_t* handle) {
  TimerCallbackData* cb_data = static_cast<TimerCallbackData*>(handle->data);
  Isolate* isolate = cb_data->isolate;

  v8::HandleScope handle_scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();

  Local<Function> callback = Local<Function>::New(isolate, cb_data->callback);

  v8::Local<v8::Value> argv[] = { v8::Undefined(isolate) };

  v8::TryCatch try_catch(isolate);
  v8::MaybeLocal<v8::Value> result = callback->Call(context, context->Global(), 1, argv);

  if (result.IsEmpty()) {
    fprintf(stderr, "JavaScript callback threw an exception in TimerCallback.\n");
    try_catch.ReThrow();
  }
  // 注意：不在此处清理 cb_data，清理在 CloseCallback 中进行
}

// 实现 LeakHandle
void LeakHandle(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  uv_loop_t* loop = node::GetCurrentEventLoop(isolate);
  assert(loop != nullptr);

  if (args.Length() < 2 || !args[1]->IsFunction()) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, "Second argument must be a function").ToLocalChecked()));
    return;
  }

  uv_timer_t* leaked_timer = new uv_timer_t;
  // 关键：设置 CloseCallback 为带清理的那个
  leaked_timer->close_cb = CloseCallback;

  void* original_data = nullptr;
  if (args[0]->IsNumber()) {
    original_data = reinterpret_cast<void*>(args[0]->IntegerValue(context).FromJust());
  }

  Local<Function> js_callback = Local<Function>::Cast(args[1]);
  // 关键：leaked_timer->data 指向新的 TimerCallbackData
  leaked_timer->data = new TimerCallbackData(isolate, js_callback, original_data);

  uv_timer_init(loop, leaked_timer);
  // 关键：使用前向声明的 TimerCallback
  uv_timer_start(leaked_timer, TimerCallback, args[0].As<Int32>()->Value()*1000, 0);
  // 关键：不调用 uv_unref，否则 TimerCallback 不会被调用
  // uv_unref(reinterpret_cast<uv_handle_t*>(leaked_timer));
}

NODE_MODULE_INIT(/*exports, module, context*/) {
  NODE_SET_METHOD(exports, "leakHandle", LeakHandle);
}