/*
 * WorkThread.cpp
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include "WorkThread.h"
#include "ThreadPool.h"
#include <unistd.h>
#include <map>
#include <fstream>
#include <sstream>

void WorkThread::regist_thread_pool(ThreadPool * p_thread_pool) {
	m_p_thread_pool = p_thread_pool; //注册到线程池
}

void WorkThread::run() {
	Task task;

	//读配置文件，获取词典文件的路径
	std::ifstream ifs_conf("./conf/config.txt");
	if(!ifs_conf)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"./conf/config.txt\" ifs OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string line, conf, file_name;
	while(getline(ifs_conf, line))
	{
		std::istringstream iss(line);
		iss >> conf;
		if(conf == "word_lib:")
		{
			iss >> file_name;
			break;
		}
	}
	//获取到词典文件路径，通过词典文件构建索引
	EditDistance ed(file_name); //定义EditDiutance对象，读取词库并构建索引到内存中

	while(true){
		m_p_thread_pool -> get_task(task, m_hash_cache);
		handle_task(task, ed);
	}
}

void WorkThread::handle_task(Task &task, EditDistance &ed) {

#ifndef NDEBUG
	std::cout << "tid:" << this->get_tid() << "(get task)--->" << task.express << std::endl;
#endif

	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	std::string send_buf;
	if(m_hash_cache.find(task.express) != m_hash_cache.end())
	  /* cache中找到缓存记录，直接返回结果 */
	{
		send_buf.clear();
		send_buf = m_hash_cache[task.express];
#ifndef NDEBUG
	std::cout << "tid:" << this->get_tid() << "(send by cache)--->" << send_buf << std::endl;
#endif

	}
	else
	  /* 没有找到匹配缓存，计算编辑距离，并将结果存入cache */
	{
		send_buf.clear();
		send_buf = ed.compute_edit_distance(task.express);
		m_hash_cache[task.express] = send_buf;
#ifndef NDEBUG
	std::cout << "tid:" << this->get_tid() << "(send by compute)--->" << send_buf << std::endl;
#endif
	}
	
	sendto(fd, send_buf.c_str(), strlen(send_buf.c_str()), 0, (struct sockaddr*)&(task.client_addr), sizeof(task.client_addr)); //发回客户端

	close(fd);
}
