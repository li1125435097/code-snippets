// 如果不是在 Windows 平台下编译以下代码
#ifndef _WIN32

// 声明一个函数 dlopen_pong，它返回一个指向字符常量的指针
const char* dlopen_pong(void);

// 声明函数 dlopen_pong 为默认可见性，使其在动态链接时可以被其他模块找到
__attribute__((visibility("default")))
// 定义 dlopen_ping 函数，它返回一个指向字符常量的指针
const char* dlopen_ping(void) {
  // 调用 dlopen_pong 函数并返回其结果
  return dlopen_pong();
}

#endif  // 结束非 Windows 平台的条件编译
