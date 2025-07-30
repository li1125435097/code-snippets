-- 创建一个admin用户
create user admin@'%' identified by '123456';

-- 查询所有用户
select user,host from mysql.user;

-- 给admin用户赋所有表权限 root用户不用输identity及之后的语句
grant all on *.* to admin@'%' identified by '123456' with grant option;