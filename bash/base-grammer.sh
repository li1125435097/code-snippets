#! /bin/bash
# bash基础语法示例

# 1.变量定义 
name='ali'
age=14

# 变量设置为只读
readonly age 
# 删除变量，只读的不能删除
unset name 

# 修改变量
age=18

# 2.输出变量
echo '$age'
echo "$age"
echo "模板字符串输出： 我的年龄是：$age"

# 3.日期格式化输出
cdate=$(date +%Y-%m-%d)
echo $cdate

# 4.字符串命令操作
echo "4.字符串命令操作输出"
cmd='ls'
cmd2='wc -l'
# 组合执行命令
echo $($cmd | $cmd2)
echo `$cmd | $cmd2`

# 5.算术运算
echo "5.算术运算输出"
sum=$((1+3-5*8+99/1))
echo $sum

# 6.数组操作
echo "6.数组操作输出"
arr=(1 2 3 4)
echo -n "${arr[1]} "
echo -n "${arr[@]} "
for v in "${arr[@]}";do
    echo -n "$v "
done
echo

# 7.条件判断
echo "7.条件判断输出"
echo "文件测试： -f(文件存在) -d(目录存在)  数值比较: -eq -ne -gt -lt  字符串比较：= != -z(空字符串判断)  判断前可加!取非"
if [ $age -ge 18 ]; then
    echo "成年人"
elif [ $age -gt 10 ]; then
    echo "青年"
else
    echo "儿童"
fi

# 8.循环
echo "8.循环输出"
# 循环简写
for v in {100..103}; do
    echo -n "$v "
done
# 调用命令
for v in $(ls); do
    echo -n " $v "
done
# 直接传数据
for v in 12 15; do
    echo -n "$v "
done

# while循环
count=1
while [ $count -lt 15 ]; do
    echo -n "$count "
    ((count++))
done
echo

# 9.函数调用
echo "9.函数调用输出，return 0表示调用成功"
greet(){
    local age1=520
    echo "hello $1 $age1"
    return 0
}
greet baby

# switch-case
case $age in
    14)
        greet 14
        ;;
    30)
        greet 30
        ;;
    *)
        echo "无效操作"
        ;;
esac


# 10.文件检查
echo "10.文件检查输出"
file='a.txt'
if [ -f "$file" ];then
    echo "文件存在"
else
    echo "文件不存在"
fi

# 11.用户输入
echo -n "11.请输入你得名字: "
read uname
echo "my name is $uname"

# 12.字符串处理
echo "12.字符串处理输出"
str='abcdefthijk'
echo "字符串长度为：${#str}"
echo "字符串截取3后5个：${str:3:5}"

# 13.命令执行状态判断
echo "13.命令执行状态判断输出"
ls /var1
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "fail"
fi

# 14.脚本参数及特殊变量
echo "14.脚本参数及特殊变量输出"
echo "脚本名称：$0 参数数量：$# 第一个参数：$1 所有参数：$*"
echo "进程id：$$ 最近后台进程id：$! 选项参数：$-"
echo "阻塞等待1秒：sleep 1"

# 15.debug模式
echo "15.debug模式输出"
set -x
debug_str='bug1'
set +x

# 16.捕捉中断信号
trap "echo '16.捕捉到中断信号!'; exit 1" SIGINT
echo "16.尝试按下ctrl+c触发，等待5秒"
sleep 5

# 17.多线程执行
echo "17.多线程执行 每个任务等5秒，多线程执行四个总共等5秒"
task(){
    greet $1
    sleep 5
}
# 后台执行
task zs &
task li &
task wmz &
task ww &
# 等待
wait


# end
echo "脚本执行结束"
exit 0