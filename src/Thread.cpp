/*
 * Thread.cpp
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#include "Thread.h"
#include <iostream>
#include <fstream>

Thread::Thread(): m_tid(0) {

}

Thread::~Thread() {
	join();
}

void Thread::start(){
	pthread_create(&m_tid, NULL, Thread::thread_function, this);
#ifndef NDEBUG
	std::cout << "tid:" << m_tid << " started~" << std::endl;
#endif
}

void * Thread::thread_function(void * arg){
	Thread * p_thread = static_cast<Thread*>(arg);
	p_thread -> run();
	return NULL;
}

void Thread::join(){
	pthread_join(m_tid, NULL);
}

pthread_t Thread::get_tid(){
	return m_tid;
}
