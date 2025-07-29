#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

uv_loop_t* loop;
uv_tcp_t server;

// 客户端上下文结构
typedef struct {
    uv_tcp_t handle;
    uv_poll_t poll_handle; // 不再需要
} client_context_t;

// 读取回调函数
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    // 分配接收缓冲区
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    client_context_t* context = (client_context_t*)stream->data;

    if (nread > 0) {
        // 成功读取数据
        printf("Received %zd bytes: %.*s\n", nread, (int)nread, buf->base);
    }
    else if (nread == UV_EOF) {
        // 客户端断开连接
        printf("Client disconnected\n");
        uv_close((uv_handle_t*)stream, (uv_close_cb)free);
    }
    else if (nread < 0) {
        // 读取错误
        fprintf(stderr, "Read error: %s\n", uv_strerror(nread));
        uv_close((uv_handle_t*)stream, (uv_close_cb)free);
    }

    // 释放缓冲区
    if (buf->base) {
        free(buf->base);
    }
}

// 客户端连接回调
void on_connection(uv_stream_t* server, int status) {
    if (status < 0) {
        fprintf(stderr, "Connection error: %s\n", uv_strerror(status));
        return;
    }

    // 创建客户端上下文（包含TCP句柄）
    client_context_t* context = (client_context_t*)malloc(sizeof(client_context_t));
    uv_tcp_init(loop, &context->handle);

    // 接受新连接
    if (uv_accept(server, (uv_stream_t*)&context->handle)) {
        free(context);
        return;
    }

    // 将上下文存储在句柄的data字段中
    context->handle.data = context;

    // 开始读取数据（替换原来的poll监控）
    uv_read_start((uv_stream_t*)&context->handle, alloc_buffer, on_read);

    // 写入数据
    uv_buf_t a[] = { {.base = "hellow world!", .len = 13} };
    uv_try_write((uv_stream_t*)&context->handle, a, 1);
}

int main() {
    loop = uv_default_loop();
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 8080, &addr);

    uv_tcp_init(loop, &server);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    uv_listen((uv_stream_t*)&server, 128, on_connection);

    printf("Server running on port 8080\n");
    return uv_run(loop, UV_RUN_DEFAULT);
}