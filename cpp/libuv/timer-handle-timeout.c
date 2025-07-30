#include <stdio.h>
#include <uv.h>

void timer_cb(uv_timer_t* timer);
int main() {
    uv_timer_t timer;
    uv_timer_init(uv_default_loop(), &timer);
    uv_timer_start(&timer, timer_cb, 2e3, 0);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}

void timer_cb(uv_timer_t* timer) {
    printf("延迟两秒输出");
}