#include <node.h>
#include <v8.h>

using namespace v8;

// 执行js脚本
void ExecuteScript(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // 检查参数
  if (args.Length() < 1 || !args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a string").ToLocalChecked()));
    return;
  }

  // 获取脚本源码
  Local<String> source = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

  // 编译脚本
  Local<Script> script;
  if (!Script::Compile(isolate->GetCurrentContext(), source).ToLocal(&script)) {
    return;  // Error compiling script
  }

  // 执行脚本
  Local<Value> result;
  if (!script->Run(isolate->GetCurrentContext()).ToLocal(&result)) {
    return;  // Error executing script
  }

  // 将其返回给JavaScript
  args.GetReturnValue().Set(result);
}

// 导出模块
void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "executeScript", ExecuteScript);
}
NODE_MODULE(js_executor, Initialize)