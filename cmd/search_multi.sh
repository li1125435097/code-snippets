# 一、根据文件名查找文件
# find
# 格式： find [path] -name [pattern]
# 举例： 
find . -name app.js    
find /var -name 'log*'

# type|which|whereis 
# 格式： 命令 可执行文件
# 举例： 
type nginx  
which grep  
whereis java


# 二、根据目录名查找目录
# find
# 格式： find [path] -type d -name [dirname]
# 举例： 
find /var -type d -name log


# 三、查找文件内容
# grep
# 格式： grep searchText file [param]
# 举例： 
grep 'Error' /var/log/nginx/*.log      
less log.gz | grep '2022/11/17' | grep 'Error' 

# awk
# 格式： awk searchText file
# 举例： 
awk '/Error/' *.log

# ag(需安装)
# 格式： ag searchText file
# 举例： 
ag 'Error' *.log   

# cut(压缩单行文件指定列查询)
# 格式： cut -c a-b file
# 举例： 
cut -c 14355-15654 app.min.js  