## Napi::Value 与其他类型的继承关系
```
Napi::Value
├── Napi::Number
│   ├── Napi::Int32
│   ├── Napi::Uint32
│   └── Napi::Double
├── Napi::String
├── Napi::Boolean
├── Napi::Object
│   ├── Napi::Array
│   ├── Napi::Function
│   └── Napi::ArrayBuffer
├── Napi::External<T>
└── Napi::Symbol
```

## Napi::Value 类型检查和转换
```
Napi::Value value = /* ... 从 JS 传入的值或从 JS 环境获取的值 ... */;

if (value.IsUndefined()) {
    // 处理 undefined
} else if (value.IsNull()) {
    // 处理 null
} else if (value.IsBoolean()) {
    // 处理 boolean
    Napi::Boolean booleanValue = value.As<Napi::Boolean>();
} else if (value.IsNumber()) {
    // 处理 number
    Napi::Number numberValue = value.As<Napi::Number>();
} else if (value.IsString()) {
    // 处理 string
    Napi::String stringValue = value.As<Napi::String>();
} else if (value.IsObject()) {
    // 处理 object
    Napi::Object objectValue = value.As<Napi::Object>();
} else if (value.IsFunction()) {
    // 处理 function
    Napi::Function functionValue = value.As<Napi::Function>();
}
// ... 其他类型检查方法
```

## Napi::Value 与其他类型的转换
用nodejs napi插件写一个函数，能接收下面十七个类型的参数，对每个参数做简单运算符操作，再把所有操作后的参数组合成Object返回给前端
Array
ArrayBuffer
ArrayBufferView
BigIntObject
BooleanObject
Date
Function
Map
NumberObject
Promise
Resolver
Proxy
RegExp
Set
SharedArrayBuffer
StringObject
SymbolObject