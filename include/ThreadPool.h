/*
 * ThreadPool.h
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "WorkThread.h"
#include "CacheThread.h"
#include "MutexLock.h"
#include "Condition.h"

class ThreadPool {
public:
	friend class CacheThread;

	ThreadPool(std::vector<WorkThread>::size_type thread_max_size);
	virtual ~ThreadPool();
	void start();
	void stop();
	void add_task(Task &task);
	void get_task(Task &task, std::unordered_map<std::string, std::string> &thread_cache);
	void get_cache(std::unordered_map<std::string, std::string> &control_cache);
	void sync_cache(std::unordered_map<std::string, std::string> &thread_cache);

private:
	std::queue<Task> m_task_queue;
	std::vector<WorkThread>::size_type m_thread_max_size;
	std::vector<WorkThread> m_thread_vec;
	CacheThread m_cache_thread;
	bool m_is_started;
	bool m_need_sync;
	int m_cache_count;

	MutexLock m_lock;
	Condition m_cond;
	MutexLock m_cache_lock;
	std::unordered_map<std::string, std::string> m_cache;
};

#endif /* THREADPOOL_H_ */
