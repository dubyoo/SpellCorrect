/*
 * MutexLock.h
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#ifndef MUTEXLOCK_H_
#define MUTEXLOCK_H_

#include <pthread.h>

class Condition;

class MutexLock {

public:
	friend class Condition;
	MutexLock();
	virtual ~MutexLock();
	void lock();
	void unlock();

private:
	pthread_mutex_t m_mutex;
};

#endif /* MUTEXLOCK_H_ */
