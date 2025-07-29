#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t* loop;
uv_process_t child_process;
uv_pipe_t stdin_pipe;
uv_pipe_t stdout_pipe;

// 子进程退出回调
void on_exit(uv_process_t* req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "\nProcess exited with status %lld, signal %d\n",
        (long long)exit_status, term_signal);
    uv_close((uv_handle_t*)req, NULL);
}

// 子进程输出读取回调
void read_stdout(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        // 打印子进程输出
        fwrite(buf->base, 1, nread, stdout);
        fflush(stdout);
    }

    if (buf->base) {
        free(buf->base);
    }

    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error: %s\n", uv_strerror(nread));
        }
        uv_close((uv_handle_t*)stream, NULL);
    }
}

// 为读取操作分配缓冲区
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

int main() {
    loop = uv_default_loop();

    // 创建管道用于标准输入输出
    uv_pipe_init(loop, &stdin_pipe, 0);
    uv_pipe_init(loop, &stdout_pipe, 0);

    // 设置进程选项
    uv_process_options_t options = { 0 };
    options.exit_cb = on_exit;
    options.file = "cmd.exe";  // Windows 使用 cmd.exe
    // Linux/macOS 使用: options.file = "/bin/sh";

    // 命令行参数（Windows 示例）
    char* args[4];
    args[0] = "cmd.exe";
    args[1] = "/c";
    args[2] = "echo Hello Libuv! && ping -n 3 127.0.0.1 && exit 42";
    args[3] = NULL;

    // Linux/macOS 替代命令：
    // args[0] = "sh";
    // args[1] = "-c";
    // args[2] = "echo 'Hello Libuv!' && sleep 2 && exit 42";
    // args[3] = NULL;

    options.args = args;
    options.flags = UV_PROCESS_WINDOWS_HIDE; // Windows 隐藏窗口

    // 标准流配置
    uv_stdio_container_t stdio[3];

    // 标准输入配置
    stdio[0].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
    stdio[0].data.stream = (uv_stream_t*)&stdin_pipe;

    // 标准输出配置（捕获子进程输出）
    stdio[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
    stdio[1].data.stream = (uv_stream_t*)&stdout_pipe;

    // 标准错误配置（重定向到标准输出）
    stdio[2].flags = UV_INHERIT_FD;
    stdio[2].data.fd = 2;

    options.stdio = stdio;
    options.stdio_count = 3;

    // 启动子进程
    fprintf(stderr, "Starting child process...\n");
    int r;
    if ((r = uv_spawn(loop, &child_process, &options))) {
        fprintf(stderr, "uv_spawn error: %s\n", uv_strerror(r));
        return 1;
    }
    fprintf(stderr, "Child process PID: %d\n", child_process.pid);

    // 开始读取子进程输出
    uv_read_start((uv_stream_t*)&stdout_pipe, alloc_buffer, read_stdout);

    // 可选的：向子进程发送输入
    // const char* message = "input data\n";
    // uv_buf_t buf = uv_buf_init((char*)message, strlen(message));
    // uv_write_t write_req;
    // uv_write(&write_req, (uv_stream_t*)&stdin_pipe, &buf, 1, NULL);

    // 运行事件循环
    uv_run(loop, UV_RUN_DEFAULT);

    // 清理资源
    uv_loop_close(loop);
    return 0;
}