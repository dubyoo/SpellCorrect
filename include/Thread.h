/*
 * Thread.h
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Thread {
public:
	Thread();
	virtual ~Thread();
	void start();
	static void * thread_function(void * arg);
	virtual void run() = 0;	//纯虚函数，由后代实现该函数的具体功能
	void join();
	pthread_t get_tid();

private:
	pthread_t m_tid;
};

#endif /* THREAD_H_ */
