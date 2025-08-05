#include <napi.h>
#include <chrono>
#include <sstream>

// 一个辅助函数：安全地获取对象的 constructor 名称
Napi::String GetConstructorName(const Napi::Value& value) {
  Napi::Env env = value.Env();
  if (value.IsObject()) {
    Napi::Object obj = value.As<Napi::Object>();
    if (obj.Has("constructor")) {
      Napi::Value ctorVal = obj.Get("constructor");
      if (ctorVal.IsFunction()) {
        Napi::Function ctor = ctorVal.As<Napi::Function>();
        if (ctor.Has("name")) {
          return ctor.Get("name").As<Napi::String>();
        }
      }
    }
  }
  return Napi::String::New(env, "Object"); // 默认
}

// 主要处理函数
Napi::Value ProcessAllTypes(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // 检查参数数量 (17个)
  if (info.Length() != 17) {
    Napi::TypeError::New(env, "Expected exactly 17 arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Object result = Napi::Object::New(env);
  Napi::Array processedArgs = Napi::Array::New(env, 17); // 用于存储处理后的17个值

  // 1. Array (假设是数字数组，求和后返回新数组)
  if (info[0].IsArray()) {
    Napi::Array jsArray = info[0].As<Napi::Array>();
    uint32_t length = jsArray.Length();
    Napi::Array newArray = Napi::Array::New(env, length);
    double sum = 0;
    for (uint32_t i = 0; i < length; i++) {
      if (jsArray.Get(i).IsNumber()) {
        double val = jsArray.Get(i).As<Napi::Number>().DoubleValue();
        sum += val;
        newArray.Set(i, Napi::Number::New(env, val + 1)); // 简单操作：每个元素+1
      } else {
        newArray.Set(i, jsArray.Get(i)); // 非数字保持原样
      }
    }
    processedArgs.Set(0, newArray);
    result.Set("ArraySum", Napi::Number::New(env, sum));
  } else {
    processedArgs.Set(0, env.Null());
    result.Set("ArraySum", env.Null());
  }

  // 2. ArrayBuffer (复制内容)
  if (info[1].IsArrayBuffer()) {
    Napi::ArrayBuffer buf = info[1].As<Napi::ArrayBuffer>();
    size_t byteLength = buf.ByteLength();
    void* data = buf.Data();
    // 创建新的 ArrayBuffer 并复制数据
    Napi::ArrayBuffer newBuf = Napi::ArrayBuffer::New(env, byteLength);
    std::memcpy(newBuf.Data(), data, byteLength);
    processedArgs.Set(1, newBuf);
  } else {
    processedArgs.Set(1, env.Null());
  }

  // 3. ArrayBufferView (例如 Int8Array, Float64Array 等) - 也进行复制
  if (info[2].IsTypedArray() || info[2].IsDataView()) { // 常见的 ArrayBufferView
    Napi::Object viewObj = info[2].As<Napi::Object>();
    // 获取 buffer 和 byteOffset, byteLength
    Napi::ArrayBuffer buffer = viewObj.Get("buffer").As<Napi::ArrayBuffer>();
    size_t byteOffset = viewObj.Get("byteOffset").As<Napi::Number>().Uint32Value();
    size_t byteLength = viewObj.Get("byteLength").As<Napi::Number>().Uint32Value();

    // 创建新的 ArrayBuffer 并复制所需部分
    Napi::ArrayBuffer newBuf = Napi::ArrayBuffer::New(env, byteLength);
    std::memcpy(newBuf.Data(), (static_cast<uint8_t*>(buffer.Data()) + byteOffset), byteLength);

    // 创建新的视图 (这里以 Uint8Array 为例，实际应根据原视图类型创建)
    Napi::Uint8Array newView = Napi::Uint8Array::New(env, byteLength, newBuf, 0);
    processedArgs.Set(2, newView);
  } else {
    processedArgs.Set(2, env.Null());
  }

  // 4. BigIntObject (如 new BigInt(123n)) - 视为 Object，检查 constructor
  if (info[3].IsObject()) {
    Napi::String ctorName = GetConstructorName(info[3]);
    if (ctorName.Utf8Value() == "BigInt") {
      // 尝试获取 valueOf() 结果 (它应该返回原始 BigInt)
      Napi::Object obj = info[3].As<Napi::Object>();
      if (obj.Has("valueOf") && obj.Get("valueOf").IsFunction()) {
        Napi::Function valueOf = obj.Get("valueOf").As<Napi::Function>();
        Napi::BigInt bigIntVal = valueOf.Call(obj, {}).As<Napi::BigInt>();
        // 简单操作：加 1n
        bool lossless;
        int64_t high = bigIntVal.Int64Value(&lossless);
        Napi::BigInt newBigInt = Napi::BigInt::New(env, static_cast<uint64_t>(high + 1));
        processedArgs.Set(3, newBigInt); // 返回原始值类型 BigInt
      } else {
        processedArgs.Set(3, env.Null());
      }
    } else {
      processedArgs.Set(3, env.Null());
    }
  } else {
    processedArgs.Set(3, env.Null());
  }

  // 5. BooleanObject (如 new Boolean(true)) - 视为 Object
  if (info[4].IsObject()) {
    Napi::String ctorName = GetConstructorName(info[4]);
    if (ctorName.Utf8Value() == "Boolean") {
      Napi::Object obj = info[4].As<Napi::Object>();
      if (obj.Has("valueOf") && obj.Get("valueOf").IsFunction()) {
        Napi::Function valueOf = obj.Get("valueOf").As<Napi::Function>();
        Napi::Value boolVal = valueOf.Call(obj, {});
        if (boolVal.IsBoolean()) {
          // 简单操作：取反
          bool newVal = !boolVal.As<Napi::Boolean>().Value();
          processedArgs.Set(4, Napi::Boolean::New(env, newVal));
        } else {
          processedArgs.Set(4, env.Null());
        }
      } else {
        processedArgs.Set(4, env.Null());
      }
    } else {
      processedArgs.Set(4, env.Null());
    }
  } else {
    processedArgs.Set(4, env.Null());
  }

  // 6. Date (加一天)
  if (info[5].IsDate()) {
    Napi::Date date = info[5].As<Napi::Date>();
    double timeMs = date.ValueOf(); // 毫秒时间戳
    // 加一天 (86400000 ms)
    Napi::Date newDate = Napi::Date::New(env, timeMs + 86400000);
    processedArgs.Set(5, newDate);
  } else {
    processedArgs.Set(5, env.Null());
  }

  // 7. Function (调用它，假设它不接受参数并返回一个值)
  if (info[6].IsFunction()) {
    Napi::Function func = info[6].As<Napi::Function>();
    Napi::Value resultVal;
    try {
      resultVal = func.Call({});
    } catch (const Napi::Error& e) {
      resultVal = env.Undefined(); // 调用出错时返回 undefined
    }
    processedArgs.Set(6, resultVal);
  } else {
    processedArgs.Set(6, env.Null());
  }

  // 8. Map (克隆)
  if (info[7].IsMap()) {
    Napi::Map jsMap = info[7].As<Napi::Map>();
    Napi::Map newMap = Napi::Map::New(env);
    // Node-API 没有直接的 Map 迭代方法，这里简化：假设我们知道键值类型或使用 JS 层循环
    // 实际中，可能需要通过 Napi::Function 调用 JS 的 forEach 或 entries()
    // 这里仅作为占位符，返回一个空 Map
    // **复杂，通常建议在 JS 层处理 Map/Set 的深度克隆**
    processedArgs.Set(7, newMap);
  } else {
    processedArgs.Set(7, env.Null());
  }

  // 9. NumberObject (如 new Number(42)) - 视为 Object
  if (info[8].IsObject()) {
    Napi::String ctorName = GetConstructorName(info[8]);
    if (ctorName.Utf8Value() == "Number") {
      Napi::Object obj = info[8].As<Napi::Object>();
      if (obj.Has("valueOf") && obj.Get("valueOf").IsFunction()) {
        Napi::Function valueOf = obj.Get("valueOf").As<Napi::Function>();
        Napi::Value numVal = valueOf.Call(obj, {});
        if (numVal.IsNumber()) {
          // 简单操作：加 1
          double newVal = numVal.As<Napi::Number>().DoubleValue() + 1;
          processedArgs.Set(8, Napi::Number::New(env, newVal));
        } else {
          processedArgs.Set(8, env.Null());
        }
      } else {
        processedArgs.Set(8, env.Null());
      }
    } else {
      processedArgs.Set(8, env.Null());
    }
  } else {
    processedArgs.Set(8, env.Null());
  }

  // 10. Promise (无法在 C++ 同步中“处理”Promise，返回原 Promise)
  if (info[9].IsPromise()) {
    // 无法同步等待，只能返回原 Promise 或创建一个新 Promise
    // 这里返回原 Promise
    processedArgs.Set(9, info[9]);
  } else {
    processedArgs.Set(9, env.Null());
  }

  // 11. Resolver (Promise Resolver) - Node-API 中没有直接的 Napi::Resolver 类
  // Resolver 通常在创建 Promise 时使用，不作为独立参数传递。这里跳过或视为 Object
  // 假设它是一个对象，不做特殊处理
  if (info[10].IsObject()) {
    // 无法进行有意义的操作，返回 null
    processedArgs.Set(10, env.Null());
  } else {
    processedArgs.Set(10, env.Null());
  }

  // 12. Proxy (获取目标对象？但 Proxy 本身是对象)
  if (info[11].IsObject()) {
    // Node-API 无法直接访问 Proxy 的 target 和 handler
    // 通常，对 Proxy 的操作会转发到目标对象
    // 这里仅返回一个表示 Proxy 的标记
    processedArgs.Set(11, Napi::String::New(env, "[Proxy]"));
  } else {
    processedArgs.Set(11, env.Null());
  }

  // 13. RegExp (提取源码和标志)
  if (info[12].IsRegExp()) {
    Napi::RegExp regExp = info[12].As<Napi::RegExp>();
    std::string source = regExp.Source();
    std::string flags = regExp.Flags();
    // 简单操作：拼接
    std::string newSource = "(?:" + source + ")";
    Napi::RegExp newRegExp = Napi::RegExp::New(env, newSource, flags);
    processedArgs.Set(12, newRegExp);
  } else {
    processedArgs.Set(12, env.Null());
  }

  // 14. Set (克隆 - 同样，Node-API 迭代困难)
  if (info[13].IsSet()) {
    Napi::Set jsSet = info[13].As<Napi::Set>();
    Napi::Set newSet = Napi::Set::New(env);
    // **复杂，同 Map，通常建议在 JS 层处理**
    // 这里返回一个空 Set
    processedArgs.Set(13, newSet);
  } else {
    processedArgs.Set(13, env.Null());
  }

  // 15. SharedArrayBuffer (复制内容到新的 SharedArrayBuffer)
  if (info[14].IsSharedArrayBuffer()) {
    Napi::SharedArrayBuffer sBuf = info[14].As<Napi::SharedArrayBuffer>();
    size_t byteLength = sBuf.ByteLength();
    void* data = sBuf.Data();
    // 创建新的 SharedArrayBuffer 并复制数据
    Napi::SharedArrayBuffer newSBuf = Napi::SharedArrayBuffer::New(env, byteLength);
    std::memcpy(newSBuf.Data(), data, byteLength);
    processedArgs.Set(14, newSBuf);
  } else {
    processedArgs.Set(14, env.Null());
  }

  // 16. StringObject (如 new String("text")) - 视为 Object
  if (info[15].IsObject()) {
    Napi::String ctorName = GetConstructorName(info[15]);
    if (ctorName.Utf8Value() == "String") {
      Napi::Object obj = info[15].As<Napi::Object>();
      if (obj.Has("valueOf") && obj.Get("valueOf").IsFunction()) {
        Napi::Function valueOf = obj.Get("valueOf").As<Napi::Function>();
        Napi::Value strVal = valueOf.Call(obj, {});
        if (strVal.IsString()) {
          // 简单操作：拼接
          std::string newVal = strVal.As<Napi::String>().Utf8Value() + " (processed)";
          processedArgs.Set(15, Napi::String::New(env, newVal));
        } else {
          processedArgs.Set(15, env.Null());
        }
      } else {
        processedArgs.Set(15, env.Null());
      }
    } else {
      processedArgs.Set(15, env.Null());
    }
  } else {
    processedArgs.Set(15, env.Null());
  }

  // 17. SymbolObject (如 new Symbol("desc")) - 视为 Object
  if (info[16].IsObject()) {
    Napi::String ctorName = GetConstructorName(info[16]);
    if (ctorName.Utf8Value() == "Symbol") {
      Napi::Object obj = info[16].As<Napi::Object>();
      if (obj.Has("valueOf") && obj.Get("valueOf").IsFunction()) {
        Napi::Function valueOf = obj.Get("valueOf").As<Napi::Function>();
        Napi::Value symVal = valueOf.Call(obj, {});
        if (symVal.IsSymbol()) {
          // Symbol 本身无法修改，返回原 Symbol
          processedArgs.Set(16, symVal);
        } else {
          processedArgs.Set(16, env.Null());
        }
      } else {
        processedArgs.Set(16, env.Null());
      }
    } else {
      processedArgs.Set(16, env.Null());
    }
  } else {
    processedArgs.Set(16, env.Null());
  }

  // 将处理后的17个参数数组放入结果对象
  result.Set("processedArguments", processedArgs);
  result.Set("success", Napi::Boolean::New(env, true));

  return result;
}

// 初始化函数
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "processAllTypes"),
             Napi::Function::New(env, ProcessAllTypes));
  return exports;
}

// 定义模块
NODE_API_MODULE(addon, Init)