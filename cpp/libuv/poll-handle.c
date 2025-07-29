//一个tcp请求客户端的代码展示poll句柄使用
#include <stdio.h>
#include <uv.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

uv_loop_t* loop;
uv_poll_t poll_handle;
SOCKET sockfd;

void poll_cb(uv_poll_t* handle, int status, int events) {
    if (status < 0) {
        fprintf(stderr, "Poll error: %s\n", uv_strerror(status));
        return;
    }

    if (events & UV_READABLE) {
        char buffer[1024];
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n > 0) {
            printf("Received: %.*s\n", n, buffer);
        }
        else if (n == 0) {
            printf("Connection closed\n");
            uv_poll_stop(handle);
        }
        else {
            fprintf(stderr, "recv error: %d\n", WSAGetLastError());
        }
    }

    if (events & UV_WRITABLE) {
        printf("Socket is writable\n");
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    loop = uv_default_loop();

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    unsigned long nonblock = 1;
    if (ioctlsocket(sockfd, FIONBIO, &nonblock) == SOCKET_ERROR) {
        fprintf(stderr, "ioctlsocket failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            fprintf(stderr, "connect failed: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
    }

    uv_poll_init_socket(loop, &poll_handle, sockfd);
    uv_poll_start(&poll_handle, UV_READABLE | UV_WRITABLE, poll_cb);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_poll_stop(&poll_handle);
    uv_close((uv_handle_t*)&poll_handle, NULL);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}