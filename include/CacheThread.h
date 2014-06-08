/*************************************************************************
	> File Name: CacheThread.h
	> Author: Dubyoo
	> Mail:wangwei1543@gmail.com 
	> Created Time: Sat 10 May 2014 12:40:37 AM CST
 ************************************************************************/

#ifndef __CACHETHREAD_H__
#define __CACHETHREAD_H__

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include "Thread.h"

class ThreadPool;

class CacheThread: public Thread
{
	public:
		void regist_thread_pool(ThreadPool * p_thread_pool);
		void run();
		void read_disk_cache();	//读磁盘cache
		void write_disk_cache();	//写磁盘cache

	private:
		ThreadPool * m_p_thread_pool;
		std::unordered_map<std::string, std::string> m_cachethread_cache; //cache
};

#endif
