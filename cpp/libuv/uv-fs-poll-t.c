#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

// 文件变化回调函数
void on_file_change(uv_fs_poll_t* handle, int status, const uv_stat_t* prev, const uv_stat_t* curr) {
    if (status < 0) {
        fprintf(stderr, "监控错误: %s\n", uv_strerror(status));
        return;
    }

    if (prev == NULL || curr == NULL) {
        // 第一次调用或无效状态
        printf("初始化监控...\n");
        return;
    }

    // 检查修改时间变化
    if (prev->st_mtim.tv_sec != curr->st_mtim.tv_sec ||
        prev->st_mtim.tv_nsec != curr->st_mtim.tv_nsec) {
        printf("文件被修改！\n");
        printf("修改时间变化: %ld.%09ld -> %ld.%09ld\n",
            prev->st_mtim.tv_sec, prev->st_mtim.tv_nsec,
            curr->st_mtim.tv_sec, curr->st_mtim.tv_nsec);
    }

    // 检查文件大小变化
    if (prev->st_size != curr->st_size) {
        printf("文件大小变化: %lu -> %lu 字节\n",
            (unsigned long)prev->st_size,
            (unsigned long)curr->st_size);
    }
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_fs_poll_t poll_handle;

    // 初始化文件轮询句柄
    uv_fs_poll_init(loop, &poll_handle);

    // 开始监控文件（每2秒检查一次）
    const char* filename = "D://codePlace//language-study//a.js"; // 监控的文件名
    int interval_ms = 2000;           // 检查间隔（毫秒）

    int r = uv_fs_poll_start(&poll_handle, on_file_change, filename, interval_ms);
    if (r) {
        fprintf(stderr, "监控启动失败: %s\n", uv_strerror(r));
        return 1;
    }

    printf("监控文件: %s (每 %d 毫秒检查一次)\n", filename, interval_ms);
    printf("修改文件并观察输出...\n");

    // 运行事件循环
    uv_run(loop, UV_RUN_DEFAULT);

    // 清理资源（实际不会执行到这里，按Ctrl+C退出）
    uv_fs_poll_stop(&poll_handle);
    uv_loop_close(loop);
    return 0;
}