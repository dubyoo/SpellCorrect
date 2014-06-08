/*
 * WorkThread.h
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "Thread.h"
#include "EditDistance.h"

class ThreadPool;

struct Task{
	std::string express;
	struct sockaddr_in client_addr;
};

class WorkThread: public Thread {
public:
	void regist_thread_pool(ThreadPool * p_thread_pool);
	void run();
	void handle_task(Task &task, EditDistance &ed);

private:
	ThreadPool * m_p_thread_pool;
	std::unordered_map<std::string, std::string> m_hash_cache;
};

#endif /* WORKTHREAD_H_ */
