#include "thread.h"
#include <stdio.h>
#include <errno.h>

CThread::CThread()
{
}

CThread::~CThread()
{
}

int CThread::Create(void *(*start_routine)(void *), void * arg)
{
	int nRet = -1;

	nRet = pthread_create(&m_tThread, 0, start_routine, arg);

	if(nRet != 0)
	{
		perror("pthread_create");
	}
	else
	{
	}

	return nRet;
}

int CThread::Join(void ** retval)
{
	// 메인스레드에서 사용. 자식 스레드에서 호출하는 pthread_exit()로부터 리턴값 수신대기
	int nRet = -1;

	nRet = pthread_join(m_tThread, retval);

	if(nRet != 0)
	{
		perror("pthread_join");
	}
	else
	{
	}

	return nRet;
}

int CThread::Detach()
{
	// 메인스레드에서 사용. 자식 스레드를 메인 스레드와 분리시키고 관련 리소스는 자식 스레드 자체적으로 운영체제에 반환(zombie가 되지 않도록 함)
	int nRet = -1;

	nRet = pthread_detach(m_tThread);

	if(nRet != 0)
	{
		perror("pthread_detach");
	}
	else
	{
	}

	return nRet;
}

void CThread::Exit()
{
	// thread의 pid를 찾아서 해당 프로세스를 kill하는... 구현하지 않음
}


// CMutex class
CMutex::CMutex()
{
	m_bLocked = false;
	pthread_mutex_init(&m_tMutex, 0);
}

CMutex::~CMutex()
{
	if(m_bLocked == true)
	{
		Unlock();
	}
	else
	{
	}

	pthread_mutex_destroy(&m_tMutex);
}

bool CMutex::Lock()
{
	int nRet = -1;

	nRet = pthread_mutex_lock(&m_tMutex);

	if(nRet != 0)
	{
		perror("pthread_mutex_lock");
	}
	else
	{
		m_bLocked = true;
	}

	return m_bLocked;
}

bool CMutex::Unlock()
{
	int nRet = -1;

	nRet = pthread_mutex_unlock(&m_tMutex);

	if(nRet != 0)
	{
		perror("pthread_mutex_unlock");
	}
	else
	{
		m_bLocked = false;
	}

	return m_bLocked;
}

