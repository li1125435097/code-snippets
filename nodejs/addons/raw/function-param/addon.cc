// addon.cc
#include <v8.h>
#include <node.h>

using namespace v8;
using namespace node;

Local<String> gstr(Isolate* isolate, char* s){
  return String::NewFromUtf8(isolate, s, NewStringType::kNormal).ToLocalChecked();
}

void ProcessTypes(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate); // 管理句柄生命周期

  // 获取当前 Context（这是关键！）
  Local<Context> context = isolate->GetCurrentContext();

  // 检查参数数量
  if (args.Length() < 17) {
    isolate->ThrowException(Exception::TypeError(gstr(isolate, "Expected 17 arguments")));
    return;
  }

  // 创建返回对象
  Local<Object> result = Object::New(isolate);

  /************** 1. undefined **************/
  if (args[0]->IsUndefined()) {
    result->Set(context, gstr(isolate,"undefined"), Undefined(isolate)).ToChecked(); // 使用 ToChecked() 简化错误处理
  }

  /************** 2. null **************/
  if (args[1]->IsNull()) {
    result->Set(context,gstr(isolate,"null"), Null(isolate)).ToChecked();
  }

  /************** 3. boolean **************/
  if (args[2]->IsBoolean()) {
    bool b = args[2].As<Boolean>()->Value();
    result->Set(context,gstr(isolate,"boolean"), Boolean::New(isolate, !b)).ToChecked();
  }

  /************** 4. int32 **************/
  if (args[3]->IsInt32()) {
    int32_t i = args[3].As<Int32>()->Value();
    result->Set(context,gstr(isolate,"sqare"), Integer::New(isolate, i * i)).ToChecked();
  }

  /************** 5. double **************/
  if (args[4]->IsNumber()) {
    double d = args[4].As<Number>()->Value();
    result->Set(context, gstr(isolate,"double"),  Number::New(isolate, d * 1.5)).ToChecked();
  }

  /************** 6. string **************/
  if (args[5]->IsString()) {
    String::Utf8Value str(isolate, args[5]);
    std::string s(*str);
    result->Set(context, gstr(isolate,"string"), gstr(isolate, const_cast<char*>((s + "_processed").c_str()))).ToChecked();
  }

  /************** 7. object (普通对象) **************/
  if (args[6]->IsObject() && !args[6]->IsArray() && !args[6]->IsDate() && !args[6]->IsRegExp() && !args[6]->IsFunction()) {
    args[6].As<Object>()->Set(context, gstr(isolate,"a"), gstr(isolate,"我改变了")).ToChecked();
    result->Set(context, gstr(isolate,"object"), args[6].As<Object>()).ToChecked();
  }

  /************** 8. array **************/
  if (args[7]->IsArray()) {
    // Local<Array> arr = Local<Array>::Cast(args[7]);
    Local<Array> arr = args[7].As<Array>();
    result->Set(context, gstr(isolate,"array_length"), Integer::New(isolate, arr->Length())).ToChecked();
  }

  /************** 9. function **************/
  if (args[8]->IsFunction()) {
    result->Set(context, gstr(isolate,"function"), gstr(isolate,"function received")).ToChecked();
  }

  /************** 10. bigint **************/
  if (args[9]->IsBigInt()) {
    Local<BigInt> big = args[9].As<BigInt>();
    int64_t value = big->Int64Value();
    Local<BigInt> new_big = BigInt::New(isolate, value + 100);
    result->Set(context, gstr(isolate,"bogint"), new_big).ToChecked();
  }

  /************** 11. symbol **************/
  if (args[10]->IsSymbol()) {
    result->Set(context, gstr(isolate, "symbol"), gstr(isolate,"received")).ToChecked();
  }

  /************** 12. date **************/
  if (args[11]->IsDate()) {
    Local<Date> date = Local<Date>::Cast(args[11]);
    double time = date->ValueOf();
    result->Set(context, gstr(isolate,"date"), Number::New(isolate, time + 1000)).ToChecked();
  }

  /************** 13. regexp **************/
  if (args[12]->IsRegExp()) {
    Local<RegExp> re = Local<RegExp>::Cast(args[12]);
    Local<Value> pattern_value;
    if (re->Get(context, gstr(isolate, "source")).ToLocal(&pattern_value)) {
      result->Set(context, gstr(isolate, "regexp"),  pattern_value).ToChecked();
    }
  }

  /************** 14. arraybuffer **************/
  if (args[13]->IsArrayBuffer()) {
    Local<ArrayBuffer> ab = args[13].As<ArrayBuffer>();
    result->Set(context, gstr(isolate, "arraybuffer"), Integer::New(isolate, static_cast<int32_t>(ab->ByteLength()))).ToChecked();
  }

  /************** 15. typedarray **************/
  if (args[14]->IsTypedArray()) {
    Local<TypedArray> ta = args[14].As<TypedArray>();
    result->Set(context, gstr(isolate, "typedarray_bytes"), Integer::New(isolate, static_cast<int32_t>(ta->ByteLength()))).ToChecked();
  }

  /************** 16. dataview **************/
  if (args[15]->IsDataView()) {
    Local<DataView> dv = args[15].As<DataView>();
    result->Set(context, gstr(isolate, "dataview_bytes"), Integer::New(isolate, static_cast<int32_t>(dv->ByteLength()))).ToChecked();
  }

  /************** 17. map **************/
  if (args[16]->IsMap()) {
    Local<Map> map = args[16].As<Map>();
    map->Set(context, gstr(isolate,"a"), gstr(isolate,"我改变了")).ToLocalChecked();
    result->Set(context, gstr(isolate, "map"), map).ToChecked();
  }

  /************** 18. set **************/
  if (args[17]->IsSet()) {
    Local<Set> set = args[17].As<Set>();
    set->Delete(context, gstr(isolate,"a")).ToChecked();
    result->Set(context, gstr(isolate, "set"), set).ToChecked();
  }

  // ✅ 返回结果
  args.GetReturnValue().Set(result);
}

// 初始化函数
void Initialize(Local<Object> target, Local<Value> module, Local<Context> context, void* priv) {
  NODE_SET_METHOD(target, "processTypes", ProcessTypes);
}

// 插件入口
NODE_MODULE(js_executor, Initialize)