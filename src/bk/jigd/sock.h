#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

// /proc/sys/net/core/rmem_max
#define SOCK_RMEM_MAX	(163840)

// /proc/sys/net/core/wmem_max
#define SOCK_WMEM_MAX	(163840)

unsigned short checksum(void * b, int len);
int ping(const char * address);

class CIpAddress
{
public:
	CIpAddress();
	~CIpAddress();

public:
	static string Bin2Str(in_addr addr);
	static in_addr_t Str2Bin(string & addr);
};

class CSocket
{
public:
	CSocket();
	~CSocket();

protected:
	int m_nFd;

public:
	int Open(int domain, int type, int protocol);
	void Close();
	int Shutdown(int how = SHUT_RDWR);
	int Errno();

public:
	int SetSockOpt(int level, int optname, int optval);
	int SetBlockMode(bool block = true);
};

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	CTcpSocket(int fd);
	~CTcpSocket();

public:
	int Open();

public:
	int Bind(string ipaddr, int port);
	int Listen(int backlog = 1024);
	int Accept(sockaddr * addr = 0, socklen_t * addrlen = 0);

public:
	int Send(void * buf, size_t len, int flags = 0);
	int Recv(void * buf, size_t len, int flags = 0);

public:
	operator int() const;
	CTcpSocket & operator = (const int & socket);
};

class CUdpSocket : public CSocket
{
public:
	CUdpSocket();
	~CUdpSocket();

public:
	int Open();

public:
	int Bind(string ipaddr, int port);

public:
	int SendTo(void * buf, size_t len, int flags, string to_ip, int to_port);
	int RecvFrom(void * buf, size_t len, int flags, string &from_ip);
};

