#include "commpkt.h"
#include <string.h>

CCommPkt::CCommPkt() : m_pPktBuf(0), m_nPktLen(0)
{
}

CCommPkt::~CCommPkt()
{
	Delete();
}

unsigned int CCommPkt::Create(unsigned int pktlen)
{
	unsigned int ret = 0;

	m_pPktBuf = new unsigned char[pktlen];

	if(m_pPktBuf == 0)
	{
		ret = 0;
	}
	else
	{
		memset(m_pPktBuf, 0, pktlen);

		m_nPktLen = pktlen;

		ret = pktlen;
	}

	return ret;
}

void CCommPkt::Delete()
{
	if(m_pPktBuf)
	{
		delete [] m_pPktBuf;
		m_pPktBuf = 0;
		m_nPktLen = 0;
	}
	else
	{
	}
}

void CCommPkt::SetLength(unsigned int pktlen)
{
	m_nPktLen = pktlen;
}

unsigned int CCommPkt::length()
{
	return m_nPktLen;
}

void CCommPkt::SetAt(unsigned int idx, unsigned char data)
{
	if(m_pPktBuf)
	{
		m_pPktBuf[idx] = data;
	}
	else
	{
	}
}

unsigned char CCommPkt::GetAt(unsigned int idx)
{
	unsigned char ret = 0;

	if(m_pPktBuf)
	{
		ret = m_pPktBuf[idx];
	}
	else
	{
	}

	return ret;
}

unsigned char * CCommPkt::buf()
{
	return m_pPktBuf;
}

char * CCommPkt::c_buf()
{
	return reinterpret_cast<char *>(m_pPktBuf);
}

CCommPkt::operator char *() const
{
	return reinterpret_cast<char *>(m_pPktBuf);
}

CCommPkt::operator short *() const
{
	return reinterpret_cast<short *>(m_pPktBuf);
}

CCommPkt::operator int *() const
{
	return reinterpret_cast<int *>(m_pPktBuf);
}

CCommPkt::operator unsigned char *() const
{
	return reinterpret_cast<unsigned char *>(m_pPktBuf);
}

CCommPkt::operator unsigned short *() const
{
	return reinterpret_cast<unsigned short *>(m_pPktBuf);
}

CCommPkt::operator unsigned int *() const
{
	return reinterpret_cast<unsigned int *>(m_pPktBuf);
}

