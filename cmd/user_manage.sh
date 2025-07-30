# 进入root用户
sudo su

# 查看用户
cat /etc/passwd

# 添加deploy用户
useradd deploy

# 设置密码
passwd deploy

# 添加root权限，vim新建文件， 输入 deploy ALL=(ALL) ALL 并保存文件即可
vim /etc/sudoers.d/deploy

# 用户组管理命令
groupmod
usermod