/*
 * Condition.cpp
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#include "Condition.h"

Condition::Condition(MutexLock &lock): m_lock(lock) {
	// TODO Auto-generated constructor stub
	pthread_cond_init(&m_cond, NULL);
}

Condition::~Condition() {
	// TODO Auto-generated destructor stub
	pthread_cond_destroy(&m_cond);
}

void Condition::wait(){
	pthread_cond_wait(&m_cond, &(m_lock.m_mutex));
}

void Condition::signal(){
	pthread_cond_signal(&m_cond);
}

void Condition::broadcast(){
	pthread_cond_broadcast(&m_cond);
}
