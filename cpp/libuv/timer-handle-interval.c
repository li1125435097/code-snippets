#include <stdio.h>
#include <uv.h>

void timer_cb(uv_timer_t* timer);
int main() {
    uv_timer_t timer;
    uv_timer_init(uv_default_loop(), &timer);
    int sec = 5;
    timer.data = &sec;
    uv_timer_start(&timer, timer_cb, 1e3, 1e3);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);    return 0;
}

void timer_cb(uv_timer_t* timer) {
    int* data = (int*)timer->data;
    if (*data == 0) {
        uv_timer_stop(timer);
    }
    printf("倒计时%d秒\n",*data);
    (*data)--;
}