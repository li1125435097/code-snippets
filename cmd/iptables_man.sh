# 查看过滤表
sudo iptables -t filter --list

# 查看其他表
sudo iptables -t mangle --list
sudo iptables -t nat --list
sudo iptables -t raw --list

# 查看现存规则
sudo iptables --list

# 清除规则
sudo iptables -t filter -F    # 单个表
sudo iptables -F    # 全部表

# 开放9000端口
sudo iptables -A INPUT -p tcp --dport 9000 -j ACCEPT
sudo iptables -A OUTPUT -p tcp --dport 9000 -j ACCEPT

# 查看端口是否开放
sudo iptables -L -n

# 规则编辑后需保存重启
service iptables save
service iptables restart

# 查看当前规则
sudo cat /etc/sysconfig/iptables
