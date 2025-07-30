# 数据库服务器使用mysqldump工具导出到数据库服务器上(animal为数据库名称，导出的是一个数据库的数据)
mysqldump -u username -p animal > animal.sql

# 数据库服务器使用mysqldump工具导出到数据库服务器上(animal为数据库名称，dog\pig为数据库下的俩表，导出的是一个数据库俩表的数据，表可以在后追加)
mysqldump -u username -p animal dog pig > dogAndPig.sql

# 多参数压缩导出(解压命令 gzip -d xxx.gz)   -B 指定数据库  -F 刷新日志  -R 备份存储过程等 -x 锁表 --master-data 在备份语句里添加CHANGE MASTER语句以及binlog文件及位置点信息
mysqldump -uroot -p -B -F -R -x --master-data=2 animal | gzip > ./animal_$(date +%F).sql.gz`

# 本地使用mysqldump工具导出到远程数据库到本地
mysqldump -h remotehost -u username -p 3306 --default-character-set=utf8 --set-gtid-purged=OFF animal > ./animal.sql

# 好用的万能语句，以下例子为导出远程animal数据库的cat表到本地(--databases参数可选择多个数据库，多个数据库用空格隔开。--tables参数可选择多个表)
mysqldump -h 10.29.5.7 -u admin -p --databases animal --tables cat > ./cat.sql

# 禅道数据库导出
/opt/zbox/run/mysql/mysqldump -P 3307 -uroot -p123456 zentao > zentao.sql