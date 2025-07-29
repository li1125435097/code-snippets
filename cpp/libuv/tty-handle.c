#include <uv.h>
#include <stdio.h>

uv_loop_t* loop;
uv_tty_t tty_in;
uv_tty_t tty_out;

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        // 回显输入内容
        uv_write_t* req = malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, (uv_stream_t*)&tty_out, &wrbuf, 1, NULL);
    }
    free(buf->base);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

int main() {
    loop = uv_default_loop();

    // 初始化输入TTY（标准输入）
    uv_tty_init(loop, &tty_in, 0, 1);
    uv_tty_set_mode(&tty_in, UV_TTY_MODE_NORMAL);

    // 初始化输出TTY（标准输出）
    uv_tty_init(loop, &tty_out, 1, 0);
    uv_tty_set_mode(&tty_out, UV_TTY_MODE_NORMAL);

    printf("Type something (Ctrl+C to exit):\n");

    // 开始读取输入
    uv_read_start((uv_stream_t*)&tty_in, alloc_buffer, on_read);

    // 运行事件循环
    uv_run(loop, UV_RUN_DEFAULT);

    // 清理
    uv_tty_reset_mode();
    uv_close((uv_handle_t*)&tty_in, NULL);
    uv_close((uv_handle_t*)&tty_out, NULL);
    uv_loop_close(loop);

    return 0;
}