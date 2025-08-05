#include <napi.h>
#include <cstring>

using namespace Napi;

Object ProcessArguments(const CallbackInfo& info) {
  Env env = info.Env();
  
  // 验证参数数量
  if (info.Length() < 17) {
    TypeError::New(env, "需要17个参数").ThrowAsJavaScriptException();
    return Object::New(env);
  }

  Object result = Object::New(env);

  // 1. Array: 添加新元素
  if (info[0].IsArray()) {
    Array arr = info[0].As<Array>();
    arr.Set(arr.Length(), "napi_added");
    result.Set("Array", arr);
  }

  // 2. ArrayBuffer: 修改第一个字节
  if (info[1].IsArrayBuffer()) {
    ArrayBuffer buf = info[1].As<ArrayBuffer>();
    if (buf.ByteLength() > 0) {
      uint8_t* data = static_cast<uint8_t*>(buf.Data());
      data[0] = 0xFF; // 设置为255
    }
    result.Set("ArrayBuffer", buf);
  }

  // 3. ArrayBufferView: 修改第一个元素
  if (info[2].IsTypedArray()) {
    TypedArray view = info[2].As<TypedArray>();
    size_t length = view.ElementLength();
    
    if (length > 0) {
      switch (view.TypedArrayType()) {
        case TypedArrayType::Uint8Array:
          static_cast<uint8_t*>(view.ArrayBuffer().Data())[view.ByteOffset()] = 255;
          break;
        case TypedArrayType::Float64Array:
          static_cast<double*>(view.ArrayBuffer().Data())[view.ByteOffset() / 8] = 3.14159;
          break;
        default: break;
      }
    }
    result.Set("ArrayBufferView", view);
  }

  // 4. BigIntObject: 值+1
  if (info[3].IsBigInt()) {
    bool lossless;
    int64_t val = info[3].As<BigInt>().Int64Value(&lossless);
    result.Set("BigIntObject", BigInt::New(env, val + 1));
  }

  // 5. BooleanObject: 取反
  if (info[4].IsBoolean()) {
    bool val = info[4].As<Boolean>();
    result.Set("BooleanObject", Boolean::New(env, !val));
  }

  // 6. Date: 增加1天
  if (info[5].IsDate()) {
    Date date = info[5].As<Date>();
    double newVal = date.ValueOf() + 86400000; // 增加1天
    result.Set("Date", Date::New(env, newVal));
  }

  // 7. Function: 调用函数
  if (info[6].IsFunction()) {
    Function fn = info[6].As<Function>();
    try {
      Value ret = fn.Call({});
      result.Set("Function", ret);
    } catch (...) {
      result.Set("Function", String::New(env, "调用失败"));
    }
  }

  // 8. Map: 添加新条目
  if (info[7].IsObject()) {
    Object mapObj = info[7].As<Object>();
    Value setFn = mapObj.Get("set");
    if (setFn.IsFunction()) {
      setFn.As<Function>().Call(mapObj, {
        String::New(env, "napi_key"),
        String::New(env, "napi_value")
      });
    }
    result.Set("Map", mapObj);
  }

  // 9. NumberObject: 值+1
  if (info[8].IsNumber()) {
    double val = info[8].As<Number>();
    result.Set("NumberObject", Number::New(env, val + 1));
  }

  // 10. Promise: 创建新Promise
  Promise::Deferred deferred = Promise::Deferred::New(env);
  deferred.Resolve(String::New(env, "新Promise"));
  result.Set("Promise", deferred.Promise());

  // 11. Resolver: 执行resolve
  if (info[10].IsFunction()) {
    Function resolver = info[10].As<Function>();
    try {
      resolver.Call({ String::New(env, "已解析") });
      result.Set("Resolver", String::New(env, "已调用"));
    } catch (...) {
      result.Set("Resolver", String::New(env, "解析失败"));
    }
  }

  // 12. Proxy: 获取目标对象
  if (info[11].IsObject()) {
    Object proxy = info[11].As<Object>();
    Value target = proxy.Get("target");
    result.Set("Proxy", target.IsUndefined() ? proxy : target);
  }

  // 13. RegExp: 创建带新标志的副本
  if (info[12].IsRegExp()) {
    RegExp regex = info[12].As<RegExp>();
    result.Set("RegExp", RegExp::New(env, regex.Source(), "g" + regex.Flags()));
  }

  // 14. Set: 添加新元素
  if (info[13].IsObject()) {
    Object setObj = info[13].As<Object>();
    Value addFn = setObj.Get("add");
    if (addFn.IsFunction()) {
      addFn.As<Function>().Call(setObj, { String::New(env, "napi_value") });
    }
    result.Set("Set", setObj);
  }

  // 15. SharedArrayBuffer: 修改第一个字节
  if (info[14].IsSharedArrayBuffer()) {
    SharedArrayBuffer sab = info[14].As<SharedArrayBuffer>();
    if (sab.ByteLength() > 0) {
      uint8_t* data = static_cast<uint8_t*>(sab.Data());
      data[0] = 0x7F; // 设置为127
    }
    result.Set("SharedArrayBuffer", sab);
  }

  // 16. StringObject: 添加后缀
  if (info[15].IsString()) {
    std::string str = info[15].As<String>();
    result.Set("StringObject", String::New(env, str + "_napi"));
  }

  // 17. SymbolObject: 创建新Symbol
  if (info[16].IsSymbol()) {
    Symbol sym = info[16].As<Symbol>();
    result.Set("SymbolObject", Symbol::New(env, "napi_symbol"));
  } else if (info[16].IsObject()) {
    result.Set("SymbolObject", Symbol::New(env, "napi_symbol"));
  }

  return result;
}

Napi::Object Init(Napi::Env env, Object exports) {
  exports.Set("processArguments", Function::New(env, ProcessArguments));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)