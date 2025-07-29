#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t* loop;
uv_fs_event_t fs_event;

void fs_event_cb(uv_fs_event_t* handle,
    const char* filename,
    int events,
    int status) {
    if (status < 0) {
        fprintf(stderr, "Error: %s\n", uv_strerror(status));
        return;
    }

    printf("Change detected in: %s\n", filename ? filename : "monitored path");
    if (events & UV_RENAME) {
        printf("Event: RENAME\n");
    }
    if (events & UV_CHANGE) {
        printf("Event: CHANGE\n");
    }
}

int main(int argc, char** argv) {
    char* filen = "D://codePlace//language-study//a.js";

    loop = uv_default_loop();
    uv_fs_event_init(loop, &fs_event);

    // 开始监控（UV_FS_EVENT_RECURSIVE可递归监控子目录）
    uv_fs_event_start(&fs_event, fs_event_cb, filen, UV_FS_EVENT_RECURSIVE);

    printf("Monitoring: %s\n", filen);
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}