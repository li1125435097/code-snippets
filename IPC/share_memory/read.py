import mmap
import os
import time

# 共享内存配置
SHM_NAME = "/my_shm"  # 必须与C程序中的名称一致
SHM_SIZE = 4096       # 必须与C程序中的大小一致

def main():
    # 1. 打开共享内存文件描述符
    try:
        # Linux/MacOS使用/dev/shm路径
        shm_fd = os.open("/dev/shm" + SHM_NAME, os.O_RDONLY)
    except FileNotFoundError:
        try:
            # Windows兼容路径
            shm_fd = os.open("C:\\Windows\\Temp" + SHM_NAME, os.O_RDONLY)
        except Exception as e:
            print(f"打开共享内存失败: {e}")
            return

    # 2. 创建内存映射
    try:
        shm_mem = mmap.mmap(shm_fd, SHM_SIZE, access=mmap.ACCESS_READ)
    except Exception as e:
        print(f"内存映射失败: {e}")
        os.close(shm_fd)
        return

    # 3. 从共享内存读取数据
    # 找到第一个空字符确定字符串结束位置
    data = shm_mem.read().split(b'\0', 1)[0]
    
    try:
        # 尝试UTF-8解码
        message = data.decode('utf-8')
    except UnicodeDecodeError:
        # 二进制数据处理
        message = f"二进制数据: {data.hex()}"
    
    print(f"Python读取到共享内存数据: {message}")
    
    # 4. 清理资源
    shm_mem.close()
    os.close(shm_fd)

if __name__ == "__main__":
    print("等待C程序写入数据...")
    time.sleep(2)  # 等待C程序初始化
    main()