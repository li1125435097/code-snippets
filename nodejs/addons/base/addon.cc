#include <node.h>
#include <v8.h>

using namespace v8;

// 执行js脚本
void FuncParamTest(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  assert(args.Length() == 4);
  assert(args[0]->IsObject());
  assert(args[1]->IsNumber());
  assert(args[2]->IsNumber());
  assert(args[3]->IsFunction());

  Local<Object> context = args[0].As<Object>();
  Local<Number> timeout = args[1].As<Number>();
  Local<Number> interval = args[2].As<Number>();
  Local<Function> fn = args[3].As<Function>();

  args.GetReturnValue().Set(context);

  // node::CallbackScope scope(isolate, args[0].As<v8::Object>(), asyncContext);
  // v8::Local<v8::Function> fn = args[3].As<v8::Function>();
  // v8::MaybeLocal<v8::Value> ret = fn->Call(isolate->GetCurrentContext(), args[0], 0, nullptr);

  // 将其返回给JavaScript
  // if (!ret.IsEmpty())
    // args.GetReturnValue().Set(ret.ToLocalChecked());
  

}

// 导出模块
void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "FuncParamTest", FuncParamTest);
}
NODE_MODULE(js_executor, Initialize)