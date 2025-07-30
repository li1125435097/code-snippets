#删除三天前一月内所有日志脚本
#! /bin/bash
cd /home/deployer/log
for j in $(seq 3 30)
do
  day=`date +%Y\-%m\-%d --date "$j days ago"`
  rm -fr *$day*
done

cd /home/deployer/media_log 
for j in $(seq 3 30)
do
  day=`date +%Y\-%m\-%d --date "$j days ago"`
  rm -fr *$day*
done