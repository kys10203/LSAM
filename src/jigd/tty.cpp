#include "tty.h"
#include <strings.h>

CTty::CTty()
{
	bzero(&m_tOldTermIos, sizeof(m_tOldTermIos));
	bzero(&m_tTermIos, sizeof(m_tTermIos));
}

CTty::~CTty()
{
	Close();
}

void CTty::InitTerm(int baudrate, int rtscts, int cs, int canon)
{
	m_tTermIos.c_cflag = CLOCAL | CREAD;

	switch(baudrate)
	{
	case 9600:
		m_tTermIos.c_cflag |= B9600;
		break;
	case 38400:
		m_tTermIos.c_cflag |= B38400;
		break;
	case 115200:
		m_tTermIos.c_cflag |= B115200;
		break;
	default:
		m_tTermIos.c_cflag |= B9600;
		break;
	}

	if(rtscts == 1)
	{
		m_tTermIos.c_cflag |= CRTSCTS;
	}
	else
	{
	}

	if(cs == 7)
	{
		m_tTermIos.c_cflag |= CS7;
	}
	else if(cs == 8)
	{
		m_tTermIos.c_cflag |= CS8;
	}
	else
	{
	}

	if(canon == 0)
	{
		m_tTermIos.c_iflag = IGNPAR;
		m_tTermIos.c_oflag = 0;
		m_tTermIos.c_lflag = 0;
		m_tTermIos.c_cc[VTIME] = 0;
		m_tTermIos.c_cc[VMIN] = 1;
	}
	else
	{
		m_tTermIos.c_iflag = IGNPAR | ICRNL;
		m_tTermIos.c_oflag = 0;
		m_tTermIos.c_lflag = ICANON;
		m_tTermIos.c_cc[VEOF] = 4;
		m_tTermIos.c_cc[VMIN] = 1;
	}

	if(m_nFd != -1)
	{
		tcflush(m_nFd, TCIFLUSH);
		tcsetattr(m_nFd, TCSANOW, &m_tTermIos);
	}
	else
	{
	}
}

bool CTty::Open(const char * path, int flag)
{
	bool ret;

	if(m_nFd != -1)
	{
		Close();
	}
	else
	{
	}

	ret = CDev::Open(path, flag);

	if(ret == true)
	{
		tcgetattr(m_nFd, &m_tOldTermIos);

		if(m_tTermIos.c_cflag == 0)
		{
			InitTerm();
		}
		else
		{
			tcflush(m_nFd, TCIFLUSH);
			tcsetattr(m_nFd, TCSANOW, &m_tTermIos);
		}
	}
	else
	{
	}

	return ret;
}

void CTty::Close()
{
	if(m_nFd != -1)
	{
		tcsetattr(m_nFd, TCSANOW, &m_tOldTermIos);
		CDev::Close();
	}
	else
	{
	}
}

