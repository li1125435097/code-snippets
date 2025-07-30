#! /bin/bash
# 所有颜色见 https://blog.csdn.net/ethnicitybeta/article/details/122601208
print() {
        if [ $2 == 'r' ]
   	  then echo "\033[31m$1\033[0m"  
	elif [ $2 == 'g' ] 
  	  then echo "\033[32m$1\033[0m"
	else echo 'illegal param'
	fi
}
print 'hellow world' 'g' 