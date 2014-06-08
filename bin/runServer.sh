#!/bin/bash

function startServer
{
	cd /home/dubyoo/Desktop/code/0504_miniSearch/ #切换到程序主目录
	./bin/miniSearchServer >> ./log/log.txt
}

function stopServer
{
	pid=$(pidof -x miniSearchServer)
	echo $pid 
	kill -9 $pid
}

function stat
{
	ps -ef | grep 'miniSearchServer'
	#echo `ps -ef | grep 'miniSearchServer'`
}

case $1 in
	start) startServer
		   echo "start!"
		;;
	stop) stopServer
		;;
	stat) stat
		;;
	*) echo "wrong cmd"
		;;
esac
