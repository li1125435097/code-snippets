#include <stdio.h>
#include <uv.h>

void wait_for_a_while();
void l();
int64_t counter = 0;

int main() {
    uv_idle_t idler;
    l("初始化idle句柄");
    uv_idle_init(uv_default_loop(), &idler);

    l("启动句柄");
    uv_idle_start(&idler, wait_for_a_while);

    l("开始循环");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    l("循环关闭");
    uv_loop_close(uv_default_loop());

    l("结束");
    return 0;
}

void wait_for_a_while(uv_idle_t* handle) {
    counter++;

    if (counter >= 30e5) {
        uv_idle_stop(handle);
        printf("300万次循环完成，关闭idle句柄\n");
    }

}

void l(char* log) {
    printf("%s\n", log);
}