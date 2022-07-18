#include "jigpkt.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

CJigPkt::CJigPkt()
{
}

CJigPkt::~CJigPkt()
{
}

unsigned int CJigPkt::Create(unsigned int msgid, unsigned int datalen, unsigned char * data)
{
	unsigned int ret = 0;

	if(m_pPktBuf == 0)
	{
		ret = CCommPkt::Create(datalen + PKT_DATA_OFFSET);

		if(ret == 0)
		{
		}
		else
		{
			*((unsigned int *)(m_pPktBuf + 0)) = htonl(START_CODE);
			*((unsigned int *)(m_pPktBuf + 4)) = htonl(msgid);
			*((unsigned int *)(m_pPktBuf + 8)) = htonl(datalen);

			if(datalen > 0 && data != 0)
			{
				memcpy(m_pPktBuf + PKT_DATA_OFFSET, data, datalen);
			}
			else
			{
			}
		}
	}
	else
	{
	}

	return ret;
}

void CJigPkt::SetMsgID(unsigned int msgid)
{
	if(m_pPktBuf != 0)
	{
		*((unsigned int *)(m_pPktBuf + 4)) = htonl(msgid);
	}
	else
	{
	}
}

unsigned int CJigPkt::GetMsgID()
{
	unsigned int ret = 0;

	if(m_pPktBuf)
	{
		ret = *((unsigned int *)(m_pPktBuf + 4));
		ret = htonl(ret);
	}
	else
	{
	}

	return ret;
}

void CJigPkt::SetDataLength(unsigned int datalen)
{
	if(m_pPktBuf != 0)
	{
		*((unsigned int *)(m_pPktBuf + 8)) = htonl(datalen);
	}
	else
	{
	}
}

unsigned int CJigPkt::GetDataLength()
{
	unsigned int ret = 0;

	if(m_pPktBuf)
	{
		ret = *((unsigned int *)(m_pPktBuf + 8));
		ret = htonl(ret);
	}
	else
	{
	}

	return ret;
}

void CJigPkt::SetDataVal(unsigned char data, unsigned int data_no)
{
	if((m_pPktBuf != 0) && (m_nPktLen >= (PKT_DATA_OFFSET + data_no + 1)))
	{
		m_pPktBuf[PKT_DATA_OFFSET + data_no] = data;
	}
	else
	{
	}
}

unsigned char CJigPkt::GetDataVal(unsigned int data_no)
{
	unsigned char ret = 0;

	if(m_pPktBuf != 0)
	{
		ret = m_pPktBuf[PKT_DATA_OFFSET + data_no];
	}
	else
	{
	}

	return ret;
}

unsigned char * CJigPkt::GetData()
{
	unsigned char * ret = 0;

	if(m_pPktBuf)
	{
		ret = m_pPktBuf + PKT_DATA_OFFSET;
	}
	else
	{
	}

	return ret;
}

bool CJigPkt::IsAvailablePacket(unsigned char * pktbuf, unsigned int pktlen)
{
	bool ret = false;

	if(pktbuf != 0)
	{
		unsigned int sc = 0;

		sc = *((unsigned int *)(pktbuf + 0));
		sc = ntohl(sc);

		if(sc == START_CODE)
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	return ret;
}

unsigned int CJigPkt::ExtractMsgID(unsigned char * pktbuf, unsigned int pktlen, bool * extract)
{
	unsigned int ret = 0;

	if(pktbuf != 0)
	{
		ret = *((unsigned int *)(pktbuf + 4));
		ret = ntohl(ret);
		*extract = true;
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned int CJigPkt::ExtractDataLength(unsigned char * pktbuf, unsigned int pktlen, bool * extract)
{
	unsigned int ret = 0;

	if(pktbuf != 0)
	{
		ret = *((unsigned int *)(pktbuf + 8));
		ret = ntohl(ret);
		*extract = true;
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned char CJigPkt::ExtractData8(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no)
{
	unsigned char ret = 0;
	bool ok = false;

	if((pktbuf != 0 && (data_no < (pktlen - PKT_DATA_OFFSET))))
	{
		ok = IsAvailablePacket(pktbuf, pktlen);

		if(ok == true)
		{
			ret = pktbuf[PKT_DATA_OFFSET + data_no];
			*extract = true;
		}
		else
		{
			*extract = false;
		}
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned short CJigPkt::ExtractData16(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no)
{
	unsigned short ret = 0;
	unsigned short * data;
	bool ok = false;

	if(pktbuf != 0)
	{
		ok = IsAvailablePacket(pktbuf, pktlen);

		if(ok == true)
		{
			data = reinterpret_cast<unsigned short *>(pktbuf + PKT_DATA_OFFSET + data_no * sizeof(unsigned short));
			ret = *data;
			*extract = true;
		}
		else
		{
			*extract = false;
		}
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned int CJigPkt::ExtractData32(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no)
{
	unsigned int ret = 0;
	unsigned int * data;
	bool ok = false;

	if(pktbuf != 0)
	{
		ok = IsAvailablePacket(pktbuf, pktlen);

		if(ok == true)
		{
			data = reinterpret_cast<unsigned int *>(pktbuf + PKT_DATA_OFFSET + data_no * sizeof(unsigned int));
			ret = *data;
			*extract = true;
		}
		else
		{
			*extract = false;
		}
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned long long CJigPkt::ExtractData64(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no)
{
	unsigned long long ret = 0;
	unsigned long long * data;
	bool ok = false;

	if(pktbuf != 0)
	{
		ok = IsAvailablePacket(pktbuf, pktlen);

		if(ok == true)
		{
			data = reinterpret_cast<unsigned long long *>(pktbuf + PKT_DATA_OFFSET + data_no * sizeof(unsigned long long));
			ret = *data;
			*extract = true;
		}
		else
		{
			*extract = false;
		}
	}
	else
	{
		*extract = false;
	}

	return ret;
}

unsigned char * CJigPkt::ExtractData(unsigned char * pktbuf, unsigned int pktlen, bool * extract)
{
	unsigned char * ret = 0;
	bool ok = false;

	ok = IsAvailablePacket(pktbuf, pktlen);

	if(ok)
	{
		ret = pktbuf + PKT_DATA_OFFSET;
		*extract = true;
	}
	else
	{
		*extract = false;
	}

	return ret;
}

