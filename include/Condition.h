/*
 * Condition.h
 *
 *  Created on: May 3, 2014
 *      Author: dubyoo
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include <pthread.h>
#include "MutexLock.h"

class Condition {
public:
	Condition(MutexLock &lock);
	virtual ~Condition();
	void wait();
	void signal();
	void broadcast();

private:
	pthread_cond_t m_cond;
	MutexLock &m_lock;
};

#endif /* CONDITION_H_ */
