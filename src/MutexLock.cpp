/*
 * MutexLock.cpp
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#include "MutexLock.h"

MutexLock::MutexLock() {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&m_mutex, NULL);
}

MutexLock::~MutexLock() {
	// TODO Auto-generated destructor stub
	pthread_mutex_destroy(&m_mutex);
}

void MutexLock::lock(){
	pthread_mutex_lock(&m_mutex);
}

void MutexLock::unlock(){
	pthread_mutex_unlock(&m_mutex);
}
