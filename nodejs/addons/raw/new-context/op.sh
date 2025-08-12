#!/bin/bash

# 文件监控函数
watch_file() {
    local target_file="app.js"
    echo "启动监控: 正在监视 $target_file 文件变化 (使用轮询检查)..."

    # 获取初始文件修改时间
    local last_mtime=$(stat -c %Y "$target_file" 2>/dev/null || stat -f %m "$target_file")
    
    # 首次执行编译+运行
    # $0 build && $0 run
    
    while true; do
        sleep 1  # 每秒检查一次
        
        # 获取当前修改时间
        local current_mtime=$(stat -c %Y "$target_file" 2>/dev/null || stat -f %m "$target_file")
        
        # 比较时间戳
        if [[ $current_mtime != $last_mtime ]]; then
            echo -e "\n检测到 $target_file 文件变化，重新编译中..."
            
            # 更新最后修改时间
            last_mtime=$current_mtime
            
            # 执行编译并在成功后运行
            if $0 build; then
                echo "编译成功，重新运行程序..."
                $0 run
            else
                echo "编译失败！跳过运行..."
            fi
            
            echo "继续监控 $target_file 文件..."
        fi
    done
}

# 参数检查
if [ $# -ne 1 ]; then
    echo "Usage: $0 <build|run|watch>"
    exit 1
fi

case $1 in
    "build")
        echo "cpp代码编译中..."
        node-gyp rebuild
        echo "cpp代码编译完成."
        ;;
    "run")
        echo "js程序运行中..."
        node app.js
        echo "js程序运行完成."
        ;;
    "watch")
        watch_file
        ;;
    *)
        echo "Invalid argument: $1"
        echo "Usage: $0 <build|run|watch>"
        exit 1
        ;;
esac