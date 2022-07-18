#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stropts.h>

class CDev
{
public:
	CDev()
	{
		m_nFd = -1;
	};

	~CDev()
	{
		Close();
	};

protected:
	int m_nFd;

public:
	int GetFd(){return m_nFd;};

public:
	virtual bool Open(const char * path, int flag = O_RDWR)
	{
		bool ret = false;

		m_nFd = open(path, flag);

		if(m_nFd < 0)
		{
			ret = false;
		}
		else
		{
			ret = true;
		}

		return ret;
	};

public:
	virtual void Close()
	{
		close(m_nFd);
		m_nFd = -1;
	};

public:
	virtual int Ioctl(unsigned int req, void * arg)
	{
		int ret = -1;

		if(m_nFd != -1)
		{
			ret = ioctl(m_nFd, req, arg);
		}
		else
		{
			ret = -1;
		}

		return ret;
	};

public:
	virtual operator int () const
	{
		return m_nFd;
	};

public:
	virtual int Read(unsigned char * buf, unsigned int buflen)
	{
		int ret = -1;

		if(m_nFd != -1)
		{
			ret = read(m_nFd, buf, buflen);
		}
		else
		{
			ret = -1;
		}

		return ret;
	};

	virtual int Write(const unsigned char * buf, unsigned int buflen)
	{
		int ret = -1;

		if(m_nFd != -1)
		{
			ret = write(m_nFd, buf, buflen);
		}
		else
		{
			ret = -1;
		}

		return ret;
	};
};

