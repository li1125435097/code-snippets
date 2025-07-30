# -------------------------------------网卡基本操作---------------------------------------
# 启用网卡
ifconfig 网卡名称 up

# 关闭网卡
ifconfig 网卡名称 down

# 设置网卡ip
ifconfig 网卡名称 ip

# 设置网卡ip和geteway
ifconfig 网卡名称 192.168.0.1 netmask 255.255.255.0

# 创建虚拟网卡，基于物理网卡创建，其数据走该网卡
# 先获取本电脑已接入的物理网卡名称(比如enps4)，<已接入网卡名称:自定义标识>组成新的网卡名称
ifconfig enps4:test 167.32.0.1




# -------------------------------------网卡信息筛选---------------------------------------
# 查看网卡信息
ifconfig

# 获取物理网卡名称
ll /sys/class/net | grep -v virtual | awk '{print $9}'

# 获取虚拟网卡名称
ll /sys/class/net | grep virtual | awk '{print $9}'

# 获取已连接的网卡
ifconfig | grep RUNNING

# 获取已连接的网卡带1行信息
ifconfig | grep RUNNING -A1

# 获取已连接的网卡名称
ifconfig | grep RUNNING | awk -F: '{print $1}'

# 获取已连接的物理网卡名称
sort -m <(ll /sys/class/net grep -v virtual | awk '{print $9}') <(ifconfig | grep RUNNING | awk -F: '{print $1}') | uniq -d

# 获取已连接的物理网卡信息
sort -m <(ll /sys/class/net grep -v virtual | awk '{print $9}') <(ifconfig | grep RUNNING | awk -F: '{print $1}') | uniq -d | xargs -I {} ifconfig {}