/**
 * @file 这是一个Node.js的C++扩展模块，用于动态加载库并调用其中的函数
 * 该模块在非Windows系统上运行
 */

 #include <node.h>      // Node.js头文件，提供用于创建Node.js扩展的API
 #include <v8.h>        // V8 JavaScript引擎头文件，提供与JavaScript交互的功能
 
 #ifndef _WIN32        // 条件编译，以下代码只在非Windows系统上编译
 
 #include <dlfcn.h>     // 动态链接库操作头文件，提供dlopen、dlsym等函数
 
 /**
  * @brief 导出函数，返回"pong"字符串
  * @return 返回"pong"字符串
  */
 extern "C"           // 指定使用C语言风格的链接
 __attribute__((visibility("default")))  // 设置符号的可见性为默认
 const char* dlopen_pong(void) {
   return "pong";
 }
 
 namespace {          // 匿名命名空间，限制符号的作用域在当前文件
 
 // 使用V8提供的类型
 using v8::FunctionCallbackInfo;  // 函数回调信息的类型
 using v8::Isolate;              // V8隔离实例，用于管理JavaScript对象
 using v8::Local;               // 局部句柄，用于管理V8对象
 using v8::Object;              // JavaScript对象类型
 using v8::String;              // JavaScript字符串类型
 using v8::Value;               // JavaScript值类型
 
 // 定义函数指针类型ping，指向一个返回const char*的无参函数
 typedef const char* (*ping)(void);
 
 // 声明一个全局函数指针
 static ping ping_func;
 
 /**
  * @brief 加载动态库并获取其中的函数指针
  * @param args JavaScript调用时传入的参数，包含动态库文件名
  */
 void LoadLibrary(const FunctionCallbackInfo<Value>& args) {
   // 将JavaScript字符串转换为UTF-8编码的C字符串
   const String::Utf8Value filename(args.GetIsolate(), args[0]);
   // 以懒加载方式打开动态库
   void* handle = dlopen(*filename, RTLD_LAZY);
   // 如果打开失败，输出错误信息
   if (handle == nullptr) fprintf(stderr, "%s\n", dlerror());
   // 确保动态库句柄有效
   assert(handle != nullptr);
   // 从动态库中获取名为"dlopen_ping"的函数
   ping_func = reinterpret_cast<ping>(dlsym(handle, "dlopen_ping"));
   // 确保找到该函数
   assert(ping_func != nullptr);
 }
 
 /**
  * @brief 调用动态库中的ping函数并返回结果
  * @param args JavaScript调用时传入的参数
  */
 void Ping(const FunctionCallbackInfo<Value>& args) {
   Isolate* isolate = args.GetIsolate();
   // 确保ping_func已初始化
   assert(ping_func != nullptr);
   // 调用ping_func并将结果转换为JavaScript字符串返回
   args.GetReturnValue().Set(String::NewFromUtf8(
         isolate, ping_func()).ToLocalChecked());
 }
 
 /**
  * @brief 初始化模块，导出函数给JavaScript使用
  * @param exports 模块导出对象
  */
 void init(Local<Object> exports) {
   // 导出load函数，用于加载动态库
   NODE_SET_METHOD(exports, "load", LoadLibrary);
   // 导出ping函数，用于调用动态库中的函数
   NODE_SET_METHOD(exports, "ping", Ping);
 }
 
 // 注册Node.js模块
 NODE_MODULE(NODE_GYP_MODULE_NAME, init)
 
 }  // anonymous namespace  // 匿名命名空间结束
 
 #endif  // _WIN32  // 条件编译结束
 