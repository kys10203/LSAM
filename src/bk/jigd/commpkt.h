#pragma once

class CCommPkt
{
public:
	CCommPkt();
	~CCommPkt();

protected:
	unsigned char * m_pPktBuf;
	unsigned int m_nPktLen;

public:
	unsigned int Create(unsigned int pktlen);
	void Delete();

public:
	void SetLength(unsigned int pktlen);
	void SetAt(unsigned int idx, unsigned char data);
	unsigned char GetAt(unsigned int idx);

public:
	unsigned int length();
	unsigned char * buf();
	char * c_buf();

public:
	operator char *() const;
	operator short *() const;
	operator int *() const;
	operator unsigned char *() const;
	operator unsigned short *() const;
	operator unsigned int *() const;
};

