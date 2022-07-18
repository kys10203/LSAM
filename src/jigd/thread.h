#pragma once

#include <pthread.h>

class CThread
{
public:
	CThread();
	~CThread();

protected:
	pthread_t m_tThread;
	pthread_attr_t m_tThreadAttr;

public:
	int Create(void *(*start_routine)(void *), void * arg);
	int Join(void ** retval = 0);
	int Detach();
	void Exit();
};

class CMutex
{
public:
	CMutex();
	~CMutex();

protected:
	pthread_mutex_t m_tMutex;
	bool m_bLocked;

public:
	bool Lock();
	bool Unlock();
};

