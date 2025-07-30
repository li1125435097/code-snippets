#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

// 共享内存配置
#define SHM_NAME "/my_shm"      // 共享内存名称
#define SHM_SIZE 4096           // 共享内存大小(4KB)

int main() {
    int shm_fd;
    void *ptr;
    
    // 1. 创建/打开共享内存对象
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. 设置共享内存大小
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }
    
    // 3. 内存映射
    ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    
    // 4. 写入数据到共享内存
    const char *message = "Hello from C! 共享内存IPC测试 - 2023";
    sprintf((char *)ptr, "%s", message);
    
    printf("C程序已写入共享内存: %s\n", (char *)ptr);
    printf("等待Python程序读取...\n");
    
    // 5. 等待读取完成(实际应用中应使用信号量同步)
    sleep(100);  // 等待10秒让Python程序读取
    
    // 6. 清理资源
    munmap(ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);  // 删除共享内存对象
    
    return 0;
}