#include <stdio.h>
#include <uv.h>

void timer_cb(uv_timer_t* timer) {
    printf("延迟两秒输出,我是1\n");
    if (timer->data != NULL){
        uv_async_t* data = (uv_async_t*)timer->data;
        data->data = "success";
        uv_async_send(data);
    }
}

void prepare_cb(uv_prepare_t* prepare) {
    printf("prepare执行，我是2\n");
    uv_prepare_stop(prepare);
    uv_close((uv_handle_t*)prepare, NULL);
}

void check_cb(uv_check_t* check) {
    printf("check执行，我是3\n");
    uv_check_stop(check);
    uv_close((uv_handle_t*)check, NULL);
}

void idle_cb(uv_idle_t* idle) {
    printf("idle执行，我是4\n");
    uv_idle_stop(idle);
    uv_close((uv_handle_t*)idle, NULL);
}

void async_cb(uv_async_t* async) {
    char* str = async->data;
    printf("async执行，接收数据是%s我是5\n",str);
    uv_close(async, NULL);
}

int main() {
    uv_check_t check;
    uv_check_init(uv_default_loop(), &check);
    uv_check_start(&check, check_cb);

    uv_prepare_t prepare;
    uv_prepare_init(uv_default_loop(), &prepare);
    uv_prepare_start(&prepare, prepare_cb);

    uv_timer_t timer;
    uv_timer_init(uv_default_loop(), &timer);
    timer.data = NULL;
    uv_timer_start(&timer, timer_cb, 0,0);

    uv_idle_t idle;
    uv_idle_init(uv_default_loop(), &idle);
    uv_idle_start(&idle, idle_cb);

    uv_async_t async;
    uv_async_init(uv_default_loop(), &async, async_cb);

    uv_timer_t timer1;
    timer1.data = &async;
    uv_timer_init(uv_default_loop(), &timer1);
    uv_timer_start(&timer1, timer_cb, 3e3, 0);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}