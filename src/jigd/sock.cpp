#include <fcntl.h>
#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "sock.h"

#define PING_PKT_SIZE	64

struct packet
{
	struct icmphdr hdr;
	char msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
};

unsigned short checksum(void * b, int len)
{
	unsigned short * buf = reinterpret_cast<unsigned short *>(b);
	unsigned int sum = 0;
	unsigned short result;

	for(sum = 0; len > 1; len -= 2)
	{
		sum += *buf++;
	}

	if(len == 1)
	{
		sum += *reinterpret_cast<unsigned char *>(buf);
	}
	else
	{
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;

	return result;
}

int ping(const char * address)
{
	int ret = -1;
	int pid = -1;
	struct protoent * proto = NULL;
	int cnt = 1;
	const int val = 255;
	int i, sd;
	struct packet pkt;
	struct sockaddr_in r_addr;
	int loop;
	struct hostent *hname;
	struct sockaddr_in addr_ping, *addr;

	pid = getpid();
	proto = getprotobyname("ICMP");
	hname = gethostbyname(address);
	bzero(&addr_ping, sizeof(addr_ping));
	addr_ping.sin_family = hname->h_addrtype;
	addr_ping.sin_port = 0;
	addr_ping.sin_addr.s_addr = *reinterpret_cast<long *>(hname->h_addr);

	addr = &addr_ping;

	sd = socket(PF_INET, SOCK_RAW, proto->p_proto);

	if(sd < 0)
	{
		perror("socket");
		ret = -1;
	}
	else
	{
		if(setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
		{
			perror("setsockopt");
			ret = -1;
		}
		else
		{
			if(fcntl(sd, F_SETFL, O_NONBLOCK) != 0)
			{
				perror("fcntl");
				ret = -1;
			}
			else
			{
				for(loop = 0; loop < 10; loop++)
				{
					int len = sizeof(r_addr);

					if(recvfrom(sd, &pkt, sizeof(pkt), 0, reinterpret_cast<struct sockaddr *>(&r_addr), reinterpret_cast<socklen_t *>(&len)) > 0)
					{
						ret = 0;
						break;
					}
					else
					{
					}

					bzero(&pkt, sizeof(pkt));
					pkt.hdr.type = ICMP_ECHO;
					pkt.hdr.un.echo.id = pid;

					for(i = 0; i < sizeof(pkt.msg) - 1; i++)
					{
						pkt.msg[i] = i + '0';
					}

					pkt.msg[i] = 0;
					pkt.hdr.un.echo.sequence = cnt++;
					pkt.hdr.checksum = checksum(&pkt, sizeof(pkt));

					if(sendto(sd, &pkt, sizeof(pkt), 0, reinterpret_cast<struct sockaddr *>(addr), sizeof(*addr)) <= 0)
					{
						perror("sendto");
						ret = -1;
					}
					else
					{
					}

					usleep(300000);
				}
			}
		}
	}

	return ret;
}

CIpAddress::CIpAddress()
{
}

CIpAddress::~CIpAddress()
{
}

string CIpAddress::Bin2Str(in_addr addr)
{
	string ret;

	ret = inet_ntoa(addr);

	return ret;
}

in_addr_t CIpAddress::Str2Bin(string & addr)
{
	in_addr_t ret;

	ret = inet_addr(addr.c_str());

	return ret;
}


CSocket::CSocket()
{
	m_nFd = -1;
}

CSocket::~CSocket()
{
}

int CSocket::Open(int domain, int type, int protocol)
{
	m_nFd = socket(domain, type, protocol);

	return m_nFd;
}

void CSocket::Close()
{
	close(m_nFd);
	m_nFd = -1;
}

int CSocket::Shutdown(int how)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		ret = shutdown(m_nFd, how);
	}
	else
	{
	}

	return ret;
}

int CSocket::Errno()
{
	return errno;
}

int CSocket::SetSockOpt(int level, int optname, int optval)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		ret = setsockopt(m_nFd, level, optname, &optval, sizeof(optval));
	}
	else
	{
	}

	return ret;
}

int CSocket::SetBlockMode(bool block)
{
	int ret = -1;
	int opt;

	if(block == true)
	{
		opt = 1;
	}
	else
	{
		opt = 0;
	}

	if(m_nFd != -1)
	{
		ret = ioctl(m_nFd, FIONBIO, &opt);
	}
	else
	{
	}

	return ret;
}


CTcpSocket::CTcpSocket()
{
}

CTcpSocket::CTcpSocket(int fd)
{
	m_nFd = fd;
}

CTcpSocket::~CTcpSocket()
{
}

int CTcpSocket::Open()
{
	return CSocket::Open(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
}

int CTcpSocket::Bind(string ipaddr, int port)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		sockaddr_in addr;

		addr.sin_addr.s_addr = CIpAddress::Str2Bin(ipaddr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		ret = bind(m_nFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
	}
	else
	{
	}

	return ret;
}

int CTcpSocket::Listen(int backlog)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		ret = listen(m_nFd, backlog);
	}
	else
	{
	}

	return ret;
}

int CTcpSocket::Accept(sockaddr * addr, socklen_t * addrlen)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		if(addr != 0 && addrlen != 0)
		{
			ret = accept(m_nFd, addr, addrlen);
		}
		else
		{
			sockaddr_in tmpaddr;
			socklen_t tmplen = sizeof(tmpaddr);

			ret = accept(m_nFd, reinterpret_cast<sockaddr *>(&tmpaddr), &tmplen);
		}
	}
	else
	{
	}

	return ret;
}

int CTcpSocket::Send(void * buf, size_t len, int flags)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		ret = send(m_nFd, buf, len, flags);
	}
	else
	{
	}

	return ret;
}

int CTcpSocket::Recv(void * buf, size_t len, int flags)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		ret = recv(m_nFd, buf, len, flags);
	}
	else
	{
	}

	return ret;
}

CTcpSocket & CTcpSocket::operator = (const int & socket)
{
	Close();

	m_nFd = socket;

	return *this;
}

CTcpSocket::operator int() const
{
	return m_nFd;
}


CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}

int CUdpSocket::Open()
{
	return CSocket::Open(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

int CUdpSocket::Bind(string ipaddr, int port)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		sockaddr_in addr;

		addr.sin_addr.s_addr = CIpAddress::Str2Bin(ipaddr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		ret = bind(m_nFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
	}
	else
	{
	}

	return ret;
}

int CUdpSocket::SendTo(void * buf, size_t len, int flags, string to_ip, int to_port)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		sockaddr_in addr;

		addr.sin_addr.s_addr = CIpAddress::Str2Bin(to_ip);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(to_port);

		ret = sendto(m_nFd, buf, len, flags, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
	}
	else
	{
	}

	return ret;
}

int CUdpSocket::RecvFrom(void * buf, size_t len, int flags, string &from_ip)
{
	int ret = -1;

	if(m_nFd != -1)
	{
		sockaddr_in addr;
		socklen_t fromlen = sizeof(addr);

		ret = recvfrom(m_nFd, buf, len, flags, reinterpret_cast<sockaddr *>(&addr), &fromlen);

		if(ret != -1)
		{
			from_ip = CIpAddress::Bin2Str(addr.sin_addr);
		}
		else
		{
		}
	}
	else
	{
	}

	return ret;
}

