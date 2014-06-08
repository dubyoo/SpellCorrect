#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ThreadPool.h"
#include "WorkThread.h"
#include "Socket.h"
#define MAX_SIZE 1024

void Daemon()
{
	const int MAXFD=64;
	if(fork()!=0)	//父进程退出
		exit(0);
	setsid();		//成为新进程组组长和新会话领导，脱离控制终端
	//chdir("/");	//此时目录不能改变，因为代码内文件路径是相对路径
	umask(0);		//重设文件访问权限掩码
	for(int i = 0; i<MAXFD; i++) //尽可能关闭所有从父进程继承来的文件
	{
		if(i == 1)	//关闭所有文件描述符，除了1，以便通过脚本运行时将输出流重定向至./log/log.txt
		{
			continue;
		}
		close(i);
	}
}

int main() {	//通过shell脚本，切换到程序主目录开始运行

	Daemon(); //守护进程
#ifndef NDEBUG
	std::cout << "====== ! Server started ! ======" << std::endl << std::endl;
#endif

	ThreadPool pool(4);	//开启4个工作线程
	pool.start();
	Task task;

	std::ifstream infs;
	infs.open("./conf/config.txt");		//读配置文件，获取IP和Port
	if(!infs)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"./conf/config.txt\" OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string line, conf, ip, port;
	while(getline(infs, line))
	{
		std::istringstream iss(line);
		iss >> conf;
		if(conf == "address:")
		{
			iss >> ip >> port;
			break;
		}
	}
#ifndef NDEBUG
	std::cout << ip << " " << port << std::endl;
#endif
	Socket mysocket(ip, port);
	infs.close();
	while(true){
		char recv_buf[MAX_SIZE];
		memset(recv_buf, 0, MAX_SIZE);
		mysocket.recv_message(recv_buf, MAX_SIZE);	//获取客户端发来的消息
#ifndef NDEBUG
		std::cout << "Server recv: " << recv_buf << std::endl;
#endif
		task.express = recv_buf;
		task.client_addr = mysocket.get_client_addr();
		pool.add_task(task);	//放入线程池的任务队列
	}

	return 0;
}
