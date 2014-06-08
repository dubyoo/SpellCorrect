/*
 * ThreadPool.cpp
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#include "ThreadPool.h"

ThreadPool::ThreadPool(std::vector<WorkThread>::size_type thread_max_size) :
		m_task_queue(), m_thread_max_size(thread_max_size), m_thread_vec(
				m_thread_max_size, WorkThread()), m_cache_thread(), m_is_started(false), m_need_sync(false), m_cache_count(0), m_lock(), m_cond(
				m_lock), m_cache_lock() {
	
	m_cache_thread.regist_thread_pool(this); //注册cache线程
	for(std::vector<WorkThread>::iterator iter = m_thread_vec.begin(); iter != m_thread_vec.end(); ++iter){
		iter -> regist_thread_pool(this); //注册工作线程
	}
}

ThreadPool::~ThreadPool() {
	for(std::vector<WorkThread>::iterator iter = m_thread_vec.begin(); iter != m_thread_vec.end(); ++iter){
		iter -> join();
	}
	m_cache_thread.join();
}

void ThreadPool::start(){ //开启线程池
	if(m_is_started == false){
		m_is_started = true;

		m_cache_thread.start();

		for(std::vector<WorkThread>::iterator iter = m_thread_vec.begin(); iter != m_thread_vec.end(); ++iter){
				iter -> start();
		}
	}
}

void ThreadPool::stop(){ //关闭线程池
	if(m_is_started == true){
		m_is_started = false;

		m_cond.broadcast();
		for(std::vector<WorkThread>::iterator iter = m_thread_vec.begin(); iter != m_thread_vec.end(); ++iter){
						iter -> join();
		}
		m_cache_thread.join();
	}
}

void ThreadPool::add_task(Task &task){
	m_lock.lock();
	if(m_is_started){
		m_task_queue.push(task);
#ifndef NDEBUG
		std::cout << "ThreadPool: task added!" << std::endl;
#endif
		m_cond.signal();
	}
	m_lock.unlock();
}

void ThreadPool::get_task(Task &task, std::unordered_map<std::string, std::string> &thread_cache){
	m_lock.lock();
	while((m_is_started && m_task_queue.empty())){
		if(m_need_sync) //WorkThread判断是否需要同步
		{
			sync_cache(thread_cache);
		}
		m_cond.wait();
	}

	if(m_is_started == false){
		m_lock.unlock();
		return;
	}

	task = m_task_queue.front();
	m_task_queue.pop();
	m_cond.signal();

	m_lock.unlock();

}

void ThreadPool::get_cache(std::unordered_map<std::string, std::string> &cachethread_cache) //CacheThread与线程池同步cache
{
	for(std::unordered_map<std::string, std::string>::iterator iter = m_cache.begin(); iter != m_cache.end(); ++iter)
		//将线程池的cache写给CacheThread
	{
		cachethread_cache[iter->first] = iter->second;
	}
	for(std::unordered_map<std::string, std::string>::iterator iter = cachethread_cache.begin(); iter != cachethread_cache.end(); ++iter)
		//将CacheThread的cache写回线程池
	{
		m_cache[iter->first] = iter->second;
	}
	m_need_sync = true; //打开同步开关，表示现在需要同步
#ifndef NDEBUG
	std::cout << "ThreadPool cache sync over!" << std::endl;
#endif
	m_cond.broadcast();
}

void ThreadPool::sync_cache(std::unordered_map<std::string, std::string> &thread_cache)	//WorkThread与线程池同步cache
{
	
	m_cache_lock.lock();
	if(m_is_started && m_need_sync)
	{
		for(std::unordered_map<std::string, std::string>::iterator iter = m_cache.begin(); iter != m_cache.end(); ++iter)
			//将线程池的cache写给WorkThread
		{
			thread_cache[iter->first] = iter->second;
		}
		for(std::unordered_map<std::string, std::string>::iterator iter = thread_cache.begin(); iter != thread_cache.end(); ++iter)
			//将WorkThread的cache写回线程池
		{
			m_cache[iter->first] = iter->second;
		}
#ifndef NDEBUG
		std::cout << "WorkThread cache sync over!" << std::endl;
#endif
		m_cache_count ++; //已同步的线程计数
		if(m_cache_count == (int)m_thread_max_size) //全部线程同步时，关闭同步开关
		{
			m_need_sync = false;
			m_cond.broadcast();
#ifndef NDEBUG
			std::cout << "------ sync over !~ ------" << std::endl;
#endif
			m_cache_count = 0;
		}
	}
	m_cache_lock.unlock();
}
