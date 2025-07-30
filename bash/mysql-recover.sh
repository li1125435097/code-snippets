# mysql命令导入(animal为数据库名称，导入的是一个数据库的数据，需确定animal数据库已建立)
mysql -u root -p animal < ./animal.sql

# mysql控制台导入(animal为数据库名称)
create database if not exists animal;
use animal;
source ./animal.sql;

# 禅道数据库导入
/opt/zbox/run/mysql/mysql -P 3307 -uroot -p123456 -D zentao < zentaonew.sql