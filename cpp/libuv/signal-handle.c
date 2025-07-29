#include <stdio.h>
#include <uv.h>

uv_loop_t *loop;
uv_signal_t signal_handle;

void signal_cb(uv_signal_t *handle, int signum) {
    printf("Received signal: %d\n", signum);
    uv_stop(uv_default_loop());  // 停止事件循环
}

int main() {
    loop = uv_default_loop();

    // 初始化信号处理器
    uv_signal_init(loop, &signal_handle);
    
    // 捕获SIGINT信号(Ctrl+C)
    uv_signal_start(&signal_handle, signal_cb, SIGINT);

    printf("Press Ctrl+C to stop...\n");
    uv_run(loop, UV_RUN_DEFAULT);

    // 清理资源
    uv_close((uv_handle_t*)&signal_handle, NULL);
    uv_loop_close(loop);

    return 0;
}