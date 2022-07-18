#include "jigapp.h"
#include "sock.h"
#include "thread.h"
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "jigpkt.h"
#include "file.h"
#include "config.h"
#include "ap471.h"
#include <iostream>

using namespace std;

// app instance
CJigApp * g_pApp = 0;

void * tty_thread(void * arg)
{
	g_pApp->SelectTtyThread();

	return 0;
}

void * dbg_thread(void * arg)
{
	g_pApp->DbgThread();

	return 0;
}

void * accept_thread(void * arg)
{
	g_pApp->AcceptThread();

	return 0;
}

void * recv_thread(void * arg)
{
	//	CJigApp * app = reinterpret_cast<CJigApp *>(arg);

	//	app->RecvThread();

	g_pApp->RecvThread();

	return 0;
}

void * pluse_thread(void * arg)
{
	g_pApp->PluseThread();

	return 0;
}

CJigApp::CJigApp()
{
	m_bRun = true;
	m_nLastAcptSock = -1;
	m_strFindStep = "";
}

CJigApp::~CJigApp()
{
	m_tAp471.Close();
}

void CJigApp::Init()
{
	m_tTtyS0.InitTerm(9600);
	if(m_tTtyS0.Open("/dev/ttyS0") == false)
	{
		cout << "/dev/ttyS0 OPEN FAILED!!!" << endl;
	}

	m_tTtyMccRs2321.InitTerm(115200);
	if(m_tTtyMccRs2321.Open("/dev/ttyXR2") == false)
	{
		cout << "/dev/ttyXR2 OPEN FAILED!!!" << endl;
	}

	m_tTtyMccRs2322.InitTerm(115200);
	if(m_tTtyMccRs2322.Open("/dev/ttyXR3") == false)
	{
		cout << "/dev/ttyXR3 OPEN FAILED!!!" << endl;
	}

	m_tTtyMccRs422.InitTerm(115200);
	if(m_tTtyMccRs422.Open("/dev/ttyXR10") == false)
	{
		cout << "/dev/ttyXR10 OPEN FAILED!!!" << endl;
	}

	m_tTtyTccRs2321.InitTerm(115200);
	if(m_tTtyTccRs2321.Open("/dev/ttyXR0") == false)
	{
		cout << "/dev/ttyXR0 OPEN FAILED!!!" << endl;
	}

	m_tTtyTccRs2322.InitTerm(115200);
	if(m_tTtyTccRs2322.Open("/dev/ttyXR1") == false)
	{
		cout << "/dev/ttyXR1 OPEN FAILED!!!" << endl;
	}

	m_tTtyTccRs422.InitTerm(115200);
	if(m_tTtyTccRs422.Open("/dev/ttyXR8") == false)
	{
		cout << "/dev/ttyXR8 OPEN FAILED!!!" << endl;
	}

	m_tTtyIobRs2321.InitTerm(115200);
	if(m_tTtyIobRs2321.Open("/dev/ttyXR7") == false)
	{
		cout << "/dev/ttyXR7 OPEN FAILED!!!" << endl;
	}

	m_tTtyIobRs2322.InitTerm(115200);
	if(m_tTtyIobRs2322.Open("/dev/ttyXR6") == false)
	{
		cout << "/dev/ttyXR6 OPEN FAILED!!!" << endl;
	}

	m_tTtyIobRs4221.InitTerm(115200);
	if(m_tTtyIobRs4221.Open("/dev/ttyXR15") == false)
	{
		cout << "/dev/ttyXR15 OPEN FAILED!!!" << endl;
	}

	m_tTtyIobRs4222.InitTerm(115200);
	if(m_tTtyIobRs4222.Open("/dev/ttyXR13") == false)
	{
		cout << "/dev/ttyXR13 OPEN FAILED!!!" << endl;
	}

	m_tTtyIobRs4223.InitTerm(115200);
	if(m_tTtyIobRs4223.Open("/dev/ttyXR14") == false)
	{
		cout << "/dev/ttyXR14 OPEN FAILED!!!" << endl;
	}

	if(m_tAp471.Open() == true)
	{
		//SetReadyLed(1);

		// mask output ports
		m_tAp471.SetMask(2, 0, 1);
		m_tAp471.SetMask(2, 1, 1);
	}
}

bool CJigApp::Run()
{
	Init();

	g_pApp = this;
	bool bRet = false;

	CThread th;
	th.Create(accept_thread, reinterpret_cast<CJigApp *>(this));
	sleep(1);

	CThread dbg;
	dbg.Create(dbg_thread, reinterpret_cast<CJigApp *>(this));
	sleep(1);

	CThread tty;
	tty.Create(tty_thread, reinterpret_cast<CJigApp *>(this));
	sleep(1);

	m_strFindStep = "0.0 BIT";
	RunBit();

	while(m_bRun)
	{
		sleep(1);
	}

	bRet = true;

	cout << "STOP JIG APP!!!" << endl;

	return bRet;
}

void CJigApp::RunBit()
{
	unsigned char data[4] = {'S', 'V', 'E', 0};

	if(m_tTtyS0.Write(data, sizeof(data) - 1) == -1)
	{
		cout << "/dev/ttyS0 WRITE FAILED!!!" << endl;
	}
	else
	{
		//DebugStringHex("[write]->", data, sizeof(data));
		cout << "[write]-> " << data << endl;
	}
}

void CJigApp::DbgThread()
{
	cout << "START DBG_THREAD!!!" << endl;

	int dump = 0;

	while(m_bRun)
	{
		string cmd;
		//cout << endl << "INPUT COMMAND: ";
		cin >> cmd;

		if(cmd == "exit")
		{
			m_bRun = false;
			m_tAp471.Close();
		}
		else if(cmd == "fsize")
		{
			string fname;
			cout << endl << "FILE NAME 입력: ";
			cin >> fname;

			cout << endl << "File Size[" << fname << "]: ";
			cout << CFile::Size(fname.c_str()) << endl << endl;
		}
		else if(cmd == "fwrite")
		{
			string fname;
			cout << endl << "FILE NAME 입력: ";
			cin >> fname;

			string text;
			cout << endl << "TEXT 입력: ";
			cin >> text;

			CFile file;
			if(file.Open(fname.c_str(), "w+") == true)
			{
				file.Write(text.c_str(), text.size());

				cout << endl << "Done." << endl;
			}
			else
			{
				cout << endl << fname << " Open Failed" << endl;
			}
			cout << "Done." << endl;
		}
		else if(cmd == "fread")
		{
			string fname;
			cout << endl << "FILE NAME 입력: ";
			cin >> fname;

			char text[256] = {0, };

			CFile file;
			if(file.Open(fname.c_str(), "r") == true)
			{
				size_t fsize = CFile::Size(fname.c_str());
				size_t readlen = 0;

				for(int i = 0; i < fsize; i += sizeof(text))
				{
					readlen = file.Read(text, sizeof(text));

					printf("fread: %s(%ld)\n", text, readlen);
				}
			}
			else
			{
				cout << endl << fname << " Open Failed" << endl;
			}
		}
		else if(cmd == "cfg")
		{
			char buf[32] = {0, };

			if(CConfig::GetConfig("test.cfg", "IPADDR", buf, sizeof(buf)) == true)
			{
				cout << "IPADDR = " << buf << endl;
			}
			else
			{
				cout << "nothing" << endl;
			}

			memset(buf, 0, sizeof(buf));
			if(CConfig::GetConfig("test.cfg", "PORT", buf, sizeof(buf)) == true)
			{
				cout << "PORT = " << buf << endl;
			}
			else
			{
				cout << "nothing" << endl;
			}

			string port(buf);
			int p = std::stoi(port) + 1;

			sprintf(buf, "%d", p);

			CConfig::SetConfig("test.cfg", "PORT", buf);

			if(CConfig::GetConfig("test.cfg", "PORT", buf, sizeof(buf)) == true)
			{
				cout << "PORT = " << buf << endl;
			}
			else
			{
				cout << "nothing" << endl;
			}
		}
		else if(cmd == "ping")
		{
			string ip;

			cout << "TARGET IP 입력: ";
			cin >> ip;

			int tmp = ping(ip.c_str());

			if(tmp == -1)
			{
				printf("PING FAILED(%d)\n", tmp);
			}
			else
			{
				printf("PING OK(%d)\n", tmp);
			}
		}
		else if(cmd == "ttywrite")
		{
			CTty tty;

			if(tty.Open("/dev/ttyS0") == false)
			{
				printf("/dev/ttyS0 OPEN FAILED!!!\n");
			}
			else
			{
			}

			string data;

			cout << "DATA 입력: ";
			cin >> data;

			tty.Write(reinterpret_cast<const unsigned char *>(data.c_str()), data.length());

			tty.Close();
			cout << "Done." << endl;
		}
		else if(cmd == "ttyread")
		{
			CTty tty;

			if(tty.Open("/dev/ttyS0") == false)
			{
				printf("/dev/ttyS0 OPEN FAILED!!!\n");
			}
			else
			{
			}

			string data;

			unsigned char buf[128] = {0, };
			int len;

			len = tty.Read(buf, sizeof(buf));

			if(len > 0)
			{
				printf("TTY READ:");
#if 0
				for(int i = 0; i < len; i++)
				{
					printf(" %02X", static_cast<unsigned char>(buf[i]));
				}
#else
				cout << buf;
#endif
				cout << endl;
			}
			else
			{
				cout << "TTY READ FAILED" << endl;
			}

			tty.Close();
		}
		else if(cmd == "getgpio")
		{
			UINT16 port, point, val;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			cout << endl << "POINT(0 - 15) 입력: ";
			cin >> point;

			val = m_tAp471.GetAt(port, point);

			printf("(%d, %d) = %04X\n", port, point, val);
		}
		else if(cmd == "setgpio")
		{
			UINT16 port, point, val;
			UINT32 tmp;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			cout << endl << "POINT(0 - 15) 입력: ";
			cin >> point;

			cout << endl << "VALUE(0 or 1) 입력: ";
			cin >> val;
			//scanf("%x", &tmp);
			//val = static_cast<UINT16>(tmp);

			m_tAp471.SetAt(port, point, val);

			dump = 1;
		}
		else if(cmd == "getgpiop")
		{
			UINT16 port, val;
			UINT32 tmp;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			val = m_tAp471.GetPort(port);

			printf("(%d) = %04X\n", port, val);

			dump = 1;
		}
		else if(cmd == "setgpiop")
		{
			UINT16 port, val;
			UINT32 tmp;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			cout << endl << "VALUE(0x0000 - 0xFFFF) 입력: ";
			scanf("%x", &tmp);
			val = static_cast<UINT16>(tmp);

			m_tAp471.SetPort(port, val);

			dump = 1;
		}
		else if(cmd == "mask")
		{
			m_tAp471.SetMask(2, 0, 1);
			m_tAp471.SetMask(2, 1, 1);
			m_tAp471.SetMask(2, 8, 1);

			cout << endl << endl << "Done." << endl;
		}
		else if(cmd == "umask")
		{
			m_tAp471.SetMask(2, 0, 0);
			m_tAp471.SetMask(2, 1, 0);
			m_tAp471.SetMask(2, 8, 0);

			cout << endl << endl << "Done." << endl;
		}
		else if(cmd == "maskgpiop")
		{
			UINT16 port, val;
			UINT32 tmp;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			cout << endl << "VALUE(0x0000 - 0xFFFF) 입력: ";
			scanf("%x", &tmp);
			val = static_cast<UINT16>(tmp);

			m_tAp471.SetMask(port, val);
			cout << "Done." << endl;
		}
		else if(cmd == "maskgpio")
		{
			UINT16 port, point, val;
			UINT32 tmp;

			cout << endl << "PORT(0 - 2) 입력: ";
			cin >> port;

			cout << endl << "POINT(0 - 15) 입력: ";
			cin >> point;

			cout << endl << "VALUE(0 or 1) 입력: ";
			cin >> val;
			//scanf("%x", &tmp);
			//val = static_cast<UINT16>(tmp);

			m_tAp471.SetMask(port, point, val);
			cout << "Done." << endl;
		}
		else if(cmd == "readyoff")
		{
			SetReadyLed(1);

			dump = 1;
		}
		else if(cmd == "readyon")
		{
			SetReadyLed(0);

			dump = 1;
		}
		else if(cmd == "31")
		{
			cout << endl << "TCC_GPIO1 TOGGLE" << endl;

			UINT16 onoff = GetTccGpio(TCC_GPIO1);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetTccGpio(TCC_GPIO1, onoff);

			dump = 1;
		}
		else if(cmd == "32")
		{
			cout << endl << "TCC_GPIO2 TOGGLE" << endl;

			UINT16 onoff = GetTccGpio(TCC_GPIO2);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetTccGpio(TCC_GPIO2, onoff);

			dump = 1;
		}
		else if(cmd == "33")
		{
			cout << endl << "TCC_GPIO3 TOGGLE" << endl;

			UINT16 onoff = GetTccGpio(TCC_GPIO3);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetTccGpio(TCC_GPIO3, onoff);

			dump = 1;
		}
		else if(cmd == "34")
		{
			cout << endl << "TCC_GPIO4 TOGGLE" << endl;

			UINT16 onoff = GetTccGpio(TCC_GPIO4);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetTccGpio(TCC_GPIO4, onoff);

			dump = 1;
		}
		else if(cmd == "41")
		{
			cout << endl << "IOB_SBC1_CARDFAIL TOGGLE" << endl;

			UINT16 onoff = GetIobGpio(IOB_SBC1_CARDFAIL);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetIobGpio(IOB_SBC1_CARDFAIL, onoff);

			dump = 1;
		}
		else if(cmd == "42")
		{
			cout << endl << "IOB_ID TOGGLE" << endl;
			UINT16 onoff = GetIobGpio(IOB_ID);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetIobGpio(IOB_ID, onoff);

			dump = 1;
		}
		else if(cmd == "43")
		{
			cout << endl << "IOB_SBC1_THERMAL TOGGLE" << endl;
			UINT16 onoff = GetIobGpio(IOB_SBC1_THERMAL);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetIobGpio(IOB_SBC1_THERMAL, onoff);

			dump = 1;
		}
		else if(cmd == "44")
		{
			cout << endl << "IOB_FAN1_SENSOR TOGGLE" << endl;
			UINT16 onoff = GetIobGpio(IOB_FAN1_SENSOR);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetIobGpio(IOB_FAN1_SENSOR, onoff);

			dump = 1;
		}
		else if(cmd == "45")
		{
			cout << endl << "IOB_STATUS_GREEN TOGGLE" << endl;
			UINT16 onoff = GetIobGpio(IOB_STATUS_GREEN);

			if(onoff == 0)
			{
				onoff = 1;
			}
			else
			{
				onoff = 0;
			}

			SetIobGpio(IOB_STATUS_GREEN, onoff);

			dump = 1;
		}
		else if(cmd == "pluse")
		{
			cout << endl << "PLUSE TEST" << endl;

			for(unsigned int i = 0; i < 500; i++)
			{
				SetIobGpio(IOB_FAN1_SENSOR, 0);
				SetIobGpio(IOB_FAN2_SENSOR, 0);
				SetIobGpio(IOB_FAN3_SENSOR, 0);
				SetIobGpio(IOB_FAN4_SENSOR, 0);
				usleep(4500);
				SetIobGpio(IOB_FAN1_SENSOR, 1);
				SetIobGpio(IOB_FAN2_SENSOR, 1);
				SetIobGpio(IOB_FAN3_SENSOR, 1);
				SetIobGpio(IOB_FAN4_SENSOR, 1);
				usleep(4500);

				if(i == 300)
				{
					unsigned char data[3] = {0x2E, 0x11, 0x3F};

					DebugStringHex("[write]->", data, sizeof(data));
					m_tTtyIobRs2321.Write(data, sizeof(data));
				}
			}
		}
		else if(cmd == "iob_h")
		{
			m_tAp471.SetPort(0, 0xff0f);
			m_tAp471.SetPort(1, 0x0000);
			m_tAp471.SetPort(2, 0x0000);
			dump = 1;
		}
		else if(cmd == "iob_l")
		{
			m_tAp471.SetPort(0, 0x000f);
			m_tAp471.SetPort(1, 0x0000);
			m_tAp471.SetPort(2, 0x0000);
			dump = 1;
		}
		else if(cmd == "iob_led")
		{
			printf("LED_GREEN = %d, LED_RED = %d\n", m_tAp471.GetAt(2, 1), m_tAp471.GetAt(2, 0));
			dump = 1;
		}
		else if(cmd == "iob_0")
		{
			m_tAp471.SetPort(0, 0x0000);
			m_tAp471.SetPort(1, 0x0000);
			m_tAp471.SetPort(2, 0x0000);
			dump = 1;
		}
		else if(cmd == "iob_f")
		{
			m_tAp471.SetPort(0, 0xffff);
			m_tAp471.SetPort(1, 0xffff);
			m_tAp471.SetPort(2, 0xffff);
			dump = 1;
		}
		else if(cmd == "dump")
		{
			dump = 1;
		}
		else if(cmd == "mcu")
		{
			cout << endl << "MCU RS232" << endl;

			BYTE cmd[3] = {'S', 'V', 'E'};

			m_tTtyS0.Write(cmd, sizeof(cmd));
		}
		else if(cmd == "shutdown")
		{
			cout << endl << "TEST JIG POWER SHUTDOWN" << endl;

			system("shutdown -P now");

			m_bRun = false;
			m_tAp471.Close();

			sleep(1);
		}
		else if(cmd == "ver")
		{
			cout << endl << "L-SAM TEST DAEMON. BETA 0.1.000(2021-07-29)" << endl;
		}
		else if(cmd == "help" || cmd == "?")
		{
			cout << "COMMAND: mcu, ver, dump, mask, umask, setgpio[p], getgpio[p], maskgpio[p], readyon, readyoff, iob_[h,l,0,f], iob_cbit, iob_led, 41, 42..." << endl;
		}
		else if(cmd == "422_o")
		{
			unsigned char data[3] = {0x2E, 0x41, 0x6F};
			m_tTtyIobRs2321.Write(data, sizeof(data));
		}
		else if(cmd == "422_x")
		{
			unsigned char data[3] = {0x2E, 0x51, 0x7F};
			m_tTtyIobRs2321.Write(data, sizeof(data));
		}
		else if(cmd == "422_1")
		{
			unsigned char data[3] = {0x00, 0x01, 0x02};
			m_tTtyIobRs4221.Write(data, sizeof(data));
		}
		else if(cmd == "422_2")
		{
			unsigned char data[3] = {0x00, 0x01, 0x02};
			m_tTtyIobRs4222.Write(data, sizeof(data));
		}
		else if(cmd == "422_3")
		{
			unsigned char data[3] = {0x00, 0x01, 0x02};
			m_tTtyIobRs4223.Write(data, sizeof(data));
		}
		else if(cmd == "iob_cbit")
		{
			unsigned char data[3] = {0x2E, 0x11, 0x3F};

			usleep(5000);
			DebugStringHex("[write]->", data, sizeof(data));
			m_tTtyIobRs2321.Write(data, sizeof(data));

			dump = 1;
		}
		else if(cmd == "test1")
		{
		}
		else
		{
		}

		if(dump == 1)
		{
			printf("[DUMP GPIO] #0: 0x%04X, #1: 0x%04X, #2: 0x%04X\n", m_tAp471.GetPort(0), m_tAp471.GetPort(1), m_tAp471.GetPort(2));

			dump = 0;
		}
	}

	cout << "STOP DBG_THREAD!!!" << endl;
}

void CJigApp::PluseThread()
{
	cout << endl << "++PLUSE THREAD" << endl;

	m_bPluseStop = false;

	for(unsigned int i = 0; i < 100; i++)
	{
		SetIobGpio(IOB_FAN1_SENSOR, 0);
		SetIobGpio(IOB_FAN2_SENSOR, 0);
		SetIobGpio(IOB_FAN3_SENSOR, 0);
		SetIobGpio(IOB_FAN4_SENSOR, 0);
		usleep(4500);
		SetIobGpio(IOB_FAN1_SENSOR, 1);
		SetIobGpio(IOB_FAN2_SENSOR, 1);
		SetIobGpio(IOB_FAN3_SENSOR, 1);
		SetIobGpio(IOB_FAN4_SENSOR, 1);
		usleep(4500);

		if(m_bPluseStop == true)
		{
			break;
		}
	}
	cout << endl << "--PLUSE THREAD" << endl;
}

void CJigApp::DebugStringHex(string chkmsg, unsigned char * buf, int len)
{
	cout << chkmsg;
	for(int i = 0; i < len; i++)
	{
		printf(" %02x", buf[i]);
	}
	cout << endl;
}

void CJigApp::SendPacket(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	int ret = -1;

	if(sock->Send(pktbuf, pktlen) != -1)
	{
		DebugStringHex("[SEND] ->", pktbuf, pktlen);
	}
	else
	{
		cout << "[SEND] -> FAILED" << endl;
	}
}

void CJigApp::SendPacket(unsigned char * pktbuf, unsigned int pktlen)
{
	int ret = -1;

	ret = send(m_nLastAcptSock, pktbuf, pktlen, 0);

	if(ret != -1)
	{
		DebugStringHex("[SEND] ->", pktbuf, pktlen);
	}
	else
	{
		cout << "[SEND] -> FAILED" << endl;
	}
}

void CJigApp::SendJigMsg(CTcpSocket * sock, unsigned int msgid, unsigned char * data, unsigned int datalen)
{
	CJigPkt pkt;

	pkt.Create(msgid, datalen, data);

	SendPacket(sock, pkt.buf(), pkt.length());
}

void CJigApp::SendJigMsg(CTcpSocket * sock, unsigned int msgid, unsigned char ok)
{
	CJigPkt pkt;

	pkt.Create(msgid, sizeof(unsigned char), &ok);

	SendPacket(sock, pkt.buf(), pkt.length());
}

void CJigApp::SendJigMsg(unsigned int msgid, unsigned char * data, unsigned int datalen)
{
	CJigPkt pkt;

	pkt.Create(msgid, datalen, data);

	SendPacket(pkt.buf(), pkt.length());
}

void CJigApp::SendJigMsg(unsigned int msgid, unsigned char ok)
{
	CJigPkt pkt;

	pkt.Create(msgid, sizeof(unsigned char), &ok);

	SendPacket(pkt.buf(), pkt.length());
}

void CJigApp::AcceptThread()
{
	cout << "START ACCEPT_THREAD!!!" << endl;

	CTcpSocket sock;
	int nAcptSock;
	sockaddr_in acptaddr;
	socklen_t acptaddr_len;

	sock.Open();
	sock.SetSockOpt(SOL_SOCKET, SO_REUSEADDR, 1);
	sock.Bind("0.0.0.0", 5000);
	sock.Listen();

	while(m_bRun)
	{
		nAcptSock = sock.Accept(reinterpret_cast<sockaddr *>(&acptaddr), &acptaddr_len);

		if(nAcptSock !=-1)
		{
			CThread th;

			cout << "[connect] from " << CIpAddress::Bin2Str(acptaddr.sin_addr) << endl;
			m_nLastAcptSock = nAcptSock;
			th.Create(recv_thread, reinterpret_cast<CJigApp *>(this));
		}
		else
		{
		}
	}

	cout << "STOP ACCEPT_THREAD!!!" << endl;
}

void CJigApp::RecvThread()
{
	cout << "START RECV_THREAD!!!" << endl;

	unsigned char buf[1024];
	int len;
	CTcpSocket sock(m_nLastAcptSock);
	unsigned int nRecvMsgID;
	bool bExtract = false;

	while(m_bRun)
	{
		len = sock.Recv(buf, sizeof(buf));

		if(len > 0)
		{
			DebugStringHex("[RECV] <-", buf, len);

			nRecvMsgID = CJigPkt::ExtractMsgID(buf, len, &bExtract);

			if(bExtract == true && CJigPkt::IsAvailablePacket(buf, len) == true)
			{
				switch(nRecvMsgID)
				{
					case PM_BIT1:
						OnRecvBit1(&sock, buf, len);
						break;
					case PM_BIT2:
						OnRecvBit2(&sock, buf, len);
						break;
					case PM_BIT3:
						OnRecvBit3(&sock, buf, len);
						break;
					case PM_BIT4:
						OnRecvBit4(&sock, buf, len);
						break;
					case PM_MCC_RS232_1:
						OnRecvMccRs2321(&sock, buf, len);
						break;
					case PM_MCC_RS232_2:
						OnRecvMccRs2322(&sock, buf, len);
						break;
					case PM_MCC_RS422:
						OnRecvMccRs422(&sock, buf, len);
						break;
					case PM_MCC_GPIO_INPUT_READY:
						OnRecvMccGpioInputReady(&sock, buf, len);
						break;
					case PM_MCC_GPIO_INPUT:
						OnRecvMccGpioInput(&sock, buf, len);
						break;
					case PM_OCC_RS232_1:
						OnRecvOccRs2321(&sock, buf, len);
						break;
					case PM_OCC_RS232_2:
						OnRecvOccRs2322(&sock, buf, len);
						break;
					case PM_OCC_RS422:
						OnRecvOccRs422(&sock, buf, len);
						break;
					case PM_OCC_GPIO_INPUT_READY:
						OnRecvOccGpioInputReady(&sock, buf, len);
						break;
					case PM_OCC_GPIO_INPUT:
						OnRecvOccGpioInput(&sock, buf, len);
						break;
					case PM_OCC_GPIO_1_READY:
						OnRecvOccGpio1Ready(&sock, buf, len);
						break;
					case PM_OCC_GPIO_1:
						OnRecvOccGpio1(&sock, buf, len);
						break;
					case PM_OCC_GPIO_2_READY:
						OnRecvOccGpio2Ready(&sock, buf, len);
						break;
					case PM_OCC_GPIO_2:
						OnRecvOccGpio2(&sock, buf, len);
						break;
					case PM_OCC_GPIO_3_READY:
						OnRecvOccGpio3Ready(&sock, buf, len);
						break;
					case PM_OCC_GPIO_3:
						OnRecvOccGpio3(&sock, buf, len);
						break;
					case PM_OCC_GPIO_4_READY:
						OnRecvOccGpio4Ready(&sock, buf, len);
						break;
					case PM_OCC_GPIO_4:
						OnRecvOccGpio4(&sock, buf, len);
						break;
					case PM_TCC_RS232_1:
						OnRecvTccRs2321(&sock, buf, len);
						break;
					case PM_TCC_RS232_2:
						OnRecvTccRs2322(&sock, buf, len);
						break;
					case PM_TCC_RS422:
						OnRecvTccRs422(&sock, buf, len);
						break;
					case PM_TCC_ID_READY:
						OnRecvTccIdReady(&sock, buf, len);
						break;
					case PM_TCC_ID:
						OnRecvTccId(&sock, buf, len);
						break;
					case PM_TCC_BIT_READY:
						OnRecvTccBitReady(&sock, buf, len);
						break;
					case PM_TCC_BIT:
						OnRecvTccBit(&sock, buf, len);
						break;
					case PM_TCC_THERMAL_READY:
						OnRecvTccThermalReady(&sock, buf, len);
						break;
					case PM_TCC_THERMAL:
						OnRecvTccThermal(&sock, buf, len);
						break;
					case PM_TCC_PCIE:
						OnRecvTccPcie(&sock, buf, len);
						break;
					case PM_IOB_DISCREATE_INPUT_HIGH:
						OnRecvIobDiscreateInputHigh(&sock, buf, len);
						break;
					case PM_IOB_DISCREATE_INPUT_LOW:
						OnRecvIobDiscreateInputLow(&sock, buf, len);
						break;
					case PM_IOB_DISCREATE_OUTPUT_HIGH:
						OnRecvIobDiscreateOutputHigh(&sock, buf, len);
						break;
					case PM_IOB_DISCREATE_OUTPUT_LOW:
						OnRecvIobDiscreateOutputLow(&sock, buf, len);
						break;
					case PM_IOB_RS232_1:
						OnRecvIobRs2321(&sock, buf, len);
						break;
					case PM_IOB_RS232_2:
						OnRecvIobRs2322(&sock, buf, len);
						break;
					case PM_IOB_RS422_1:
						OnRecvIobRs4221(&sock, buf, len);
						break;
					case PM_IOB_RS422_2:
						OnRecvIobRs4222(&sock, buf, len);
						break;
					case PM_IOB_RS422_3:
						OnRecvIobRs4223(&sock, buf, len);
						break;
					case PM_IOB_12V:
						OnRecvIob12v(&sock, buf, len);
						break;
#if 0
					case PM_IOB_CBIT:
						OnRecvIobCbit(&sock, buf, len);
						break;
					case PM_IOB_LED:
						OnRecvIobLed(&sock, buf, len);
						break;
					case PM_IOB_BIT_READY:
						OnRecvIobBitReady(&sock, buf, len);
						break;
					case PM_IOB_BIT:
						OnRecvIobBit(&sock, buf, len);
						break;
					case PM_IOB_ID_READY:
						OnRecvIobIdReady(&sock, buf, len);
						break;
					case PM_IOB_ID:
						OnRecvIobId(&sock, buf, len);
						break;
					case PM_IOB_THERMAL_READY:
						OnRecvIobThermalReady(&sock, buf, len);
						break;
					case PM_IOB_THERMAL:
						OnRecvIobThermal(&sock, buf, len);
						break;
					case PM_IOB_FANSENSOR_READY:
						OnRecvIobFansensorReady(&sock, buf, len);
						break;
					case PM_IOB_FANSENSOR:
						OnRecvIobFansensor(&sock, buf, len);
						break;
					case PM_IOB_LED_READY:
						OnRecvIobLedReady(&sock, buf, len);
						break;
					case PM_IOB_LED:
						OnRecvIobLed(&sock, buf, len);
						break;
					case PM_IOB_12V_READY:
						OnRecvIob12vReady(&sock, buf, len);
						break;
					case PM_IOB_12V:
						OnRecvIob12v(&sock, buf, len);
						break;
#endif
					case PM_SHUTDOWN:
						SendJigMsg(PM_SHUTDOWN + 0x80000000, DEF_OK);
						sleep(1);
						system("shutdown -P now");
						break;
					default:
						break;
				}
			}
			else
			{
			}
		}
		else if(len == 0)
		{
			cout << "[disconnect]" << endl; 
			break;
		}
		else
		{
			cout << "[RECV] FAILED!!!(errno = " << errno << ")" << endl;
			break;
		}
	}

	sock.Shutdown();
	sock.Close();

	cout << "STOP RECV_THREAD!!!" << endl;
}

void CJigApp::OnRecvBit1(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvBit1()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		m_strFindStep = "0.1 BIT";
		RunBit();
		//SendJigMsg(sock, PM_BIT1 + 0x80000000, reinterpret_cast<unsigned char *>(const_cast<char *>(m_strVolt[0].c_str())), m_strVolt[0].length());
	}

	cout << "--OnRecvBit1()" << endl << endl;
}

void CJigApp::OnRecvBit2(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvBit2()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		m_strFindStep = "0.2 BIT";
		RunBit();
	}
#if 0
	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(JIG_READY));

	if(data == 1)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	SendJigMsg(sock, PM_BIT2 + 0x80000000, data);
#endif
	cout << "--OnRecvBit2()" << endl << endl;
}

void CJigApp::OnRecvBit3(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvBit3()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		m_strFindStep = "0.3 BIT";
		RunBit();
	}

	cout << "--OnRecvBit3()" << endl << endl;
}

void CJigApp::OnRecvBit4(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvBit4()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		m_strFindStep = "0.4 BIT";
		RunBit();
	}

	cout << "--OnRecvBit4()" << endl << endl;
}

void CJigApp::OnRecvMccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvMccRs2321()" << endl;

	unsigned char data[13] = {0xaa, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01};


	SendJigMsg(sock, PM_MCC_RS232_1 + 0x80000000, data, sizeof(data));

#if 0
	bool ok = false;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		const unsigned char * buf = const_cast<const unsigned char *>(CJigPkt::ExtractData(pktbuf, pktlen, &ok));
		unsigned int len = CJigPkt::ExtractDataLength(pktbuf, pktlen, &ok);

		m_tTtyMccRs2321.Write(buf, len);
		DebugStringHex("[write]->", CJigPkt::ExtractData(pktbuf, pktlen, &ok), len);
	}
	else
	{
		SendJigMsg(sock, PM_MCC_RS232_1 + 0x80000000, 0));
	}
#endif

	cout << "--OnRecvMccRs2321()" << endl << endl;
}

void CJigApp::OnRecvMccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvMccRs2322()" << endl;

	SendJigMsg(sock, PM_MCC_RS232_2 + 0x80000000, 1);

	cout << "--OnRecvMccRs2322()" << endl << endl;
}

void CJigApp::OnRecvMccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvMccRs422()" << endl;

	SendJigMsg(sock, PM_MCC_RS422 + 0x80000000, 1);

	cout << "--OnRecvMccRs422()" << endl << endl;
}

void CJigApp::OnRecvMccGpioInputReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvMccGpioInputReady()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(JIG_READY));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	SendJigMsg(sock, PM_MCC_GPIO_INPUT_READY + 0x80000000, data);

	cout << "--OnRecvMccGpioInputReady()" << endl << endl;
}

void CJigApp::OnRecvMccGpioInput(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvMccGpioInput()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(JIG_READY));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	SendJigMsg(sock, PM_MCC_GPIO_INPUT + 0x80000000, data);

	cout << "--OnRecvMccGpioInput()" << endl << endl;
}

void CJigApp::OnRecvOccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccRs2321()" << endl;
#if 0
	bool ok = false;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		const unsigned char * buf = const_cast<const unsigned char *>(CJigPkt::ExtractData(pktbuf, pktlen, &ok));
		unsigned int len = CJigPkt::ExtractDataLength(pktbuf, pktlen, &ok);

		m_tTtyOccRs2321.Write(buf, len);
		DebugStringHex("[write]->", CJigPkt::ExtractData(pktbuf, pktlen, &ok), len);
	}
	else
	{
		unsigned char data;

		data = 0;

		CJigPkt pkt;
		pkt.Create(PM_OCC_RS232_1 + 0x80000000, sizeof(data), &data);
		SendPacket(sock, pkt.buf(), pkt.length());
	}
#else
	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_OCC_RS232_1 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());
#endif

	cout << "--OnRecvOccRs2321()" << endl << endl;
}

void CJigApp::OnRecvOccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccRs2322()" << endl;

	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_OCC_RS232_2 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccRs2322()" << endl << endl;
}

void CJigApp::OnRecvOccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccRs422()" << endl;

	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_OCC_RS422 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccRs422()" << endl << endl;
}

void CJigApp::OnRecvOccGpioInputReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpioInputReady()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_INPUT_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpioInputReady()" << endl << endl;
}

void CJigApp::OnRecvOccGpioInput(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpioInput()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_INPUT + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpioInput()" << endl << endl;
}

void CJigApp::OnRecvOccGpio1Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio1Ready()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_1_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio1Ready()" << endl << endl;
}

void CJigApp::OnRecvOccGpio1(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio1()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_1 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio1()" << endl << endl;
}

void CJigApp::OnRecvOccGpio2Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio2Ready()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO2));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_2_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio2Ready()" << endl << endl;
}

void CJigApp::OnRecvOccGpio2(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio2()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO2));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_2 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio2()" << endl << endl;
}

void CJigApp::OnRecvOccGpio3Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio3Ready()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO3));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_3_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio3Ready()" << endl << endl;
}

void CJigApp::OnRecvOccGpio3(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio3()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO3));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_3 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio3()" << endl << endl;
}

void CJigApp::OnRecvOccGpio4Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio4Ready()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO4));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_4_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio4Ready()" << endl << endl;
}

void CJigApp::OnRecvOccGpio4(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvOccGpio4()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(OCC_GPIO4));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_OCC_GPIO_4 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvOccGpio4()" << endl << endl;
}

void CJigApp::OnRecvTccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccRs2321()" << endl;

	bool ok = false;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		const unsigned char * buf = const_cast<const unsigned char *>(CJigPkt::ExtractData(pktbuf, pktlen, &ok));
		unsigned int len = CJigPkt::ExtractDataLength(pktbuf, pktlen, &ok);

		m_tTtyTccRs2321.Write(buf, len);
		DebugStringHex("[write]->", CJigPkt::ExtractData(pktbuf, pktlen, &ok), len);
	}
	else
	{
		unsigned char data;

		data = 0;

		CJigPkt pkt;
		pkt.Create(PM_TCC_RS232_1 + 0x80000000, sizeof(data), &data);
		SendPacket(sock, pkt.buf(), pkt.length());
	}
#if 0
	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_TCC_RS232_1 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());
#endif
	cout << "--OnRecvTccRs2321()" << endl << endl;
}

void CJigApp::OnRecvTccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccRs2322()" << endl;

	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_TCC_RS232_2 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccRs2322()" << endl << endl;
}

void CJigApp::OnRecvTccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccRs422()" << endl;

	unsigned char data;

	data = 1;

	CJigPkt pkt;

	pkt.Create(PM_TCC_RS422 + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccRs422()" << endl << endl;
}

void CJigApp::OnRecvTccIdReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccIdReady()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_ID_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccIdReady()" << endl << endl;
}

void CJigApp::OnRecvTccId(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccId()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO1));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_ID + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccId()" << endl << endl;
}

void CJigApp::OnRecvTccBitReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccBitReady()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO2));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_BIT_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccBitReady()" << endl << endl;
}

void CJigApp::OnRecvTccBit(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccBit()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO2));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_BIT + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccBit()" << endl << endl;
}

void CJigApp::OnRecvTccThermalReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccThermalReady()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO3));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_THERMAL_READY + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccThermalReady()" << endl << endl;
}

void CJigApp::OnRecvTccThermal(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccThermal()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO3));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_THERMAL + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccThermal()" << endl << endl;
}

void CJigApp::OnRecvTccPcie(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvTccPcie()" << endl;

	unsigned char data;

	data = static_cast<unsigned char>(GetGpio(TCC_GPIO4));

	if(data == 0)
	{
		data = 1;
	}
	else
	{
		data = 0;
	}

	CJigPkt pkt;

	pkt.Create(PM_TCC_PCIE + 0x80000000, sizeof(data), &data);

	SendPacket(sock, pkt.buf(), pkt.length());

	cout << "--OnRecvTccPcie()" << endl << endl;
}

BOOL CJigApp::SetIobGpioInputHigh()
{
	BOOL ret = FALSE;

	//if(m_tAp471.SetPort(0, 0xFF0F) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
	if(m_tAp471.SetPort(0, 0xFFFF) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
	{
		CThread th;

		th.Create(pluse_thread, reinterpret_cast<CJigApp *>(this));

		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	return ret;
}

BOOL CJigApp::SetIobGpioInputLow()
{
	BOOL ret = FALSE;

	//if(m_tAp471.SetPort(0, 0x000F) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
	if(m_tAp471.SetPort(0, 0x0000) == TRUE && m_tAp471.SetPort(1, 0x000F) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	return ret;
}

void CJigApp::OnRecvIobDiscreateInputHigh(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobDiscreateInputHigh()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		// HIGH(=1)이면 LED 꺼짐, LOW(=0)이면 LED 켜짐 
		//if(m_tAp471.SetPort(0, 0xFFFF) == TRUE && m_tAp471.SetPort(1, 0x0010) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		if(SetIobGpioInputHigh() == TRUE)
		{
			unsigned char data[3] = {0x2E, 0x11, 0x3F};

			usleep(5000);
			DebugStringHex("[write]->", data, sizeof(data));

			if(m_tTtyIobRs2321.Write(data, sizeof(data)) == -1)
			{
				// RS232 통신 오류 시, 실패 응답
				SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_HIGH + 0x80000000, DEF_FAIL);
			}
			else
			{
				SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_HIGH + 0x80000000, DEF_OK);
			}
		}
		else
		{
			SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_HIGH + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobDiscreateInputHigh()" << endl;
}

void CJigApp::OnRecvIobDiscreateInputLow(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobDiscreateInputLow()" << endl;

	m_bPluseStop = true;
	usleep(1000000);

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		// HIGH(=1)이면 LED 꺼짐, LOW(=0)이면 LED 켜짐 
		//if(m_tAp471.SetPort(0, 0x0000) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		if(SetIobGpioInputLow() == TRUE)
		{
			unsigned char data[3] = {0x2E, 0x11, 0x3F};

			usleep(5000);
			DebugStringHex("[write]->", data, sizeof(data));

			if(m_tTtyIobRs2321.Write(data, sizeof(data)) == -1)
			{
				// RS232 통신 오류 시, 실패 응답
				SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_LOW + 0x80000000, DEF_FAIL);
			}
			else
			{
				SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_LOW + 0x80000000, DEF_OK);
			}
		}
		else
		{
			SendJigMsg(sock, PM_IOB_DISCREATE_INPUT_LOW + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobDiscreateInputLow()" << endl;
}

void CJigApp::OnRecvIobDiscreateOutputLow(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobDiscreateOutputLow()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		// RED LED 조건 -> 이산신호 입력 LOW와 다름
		if(m_tAp471.SetPort(0, 0x000F) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		{
			unsigned char data[2] = {0, 0};

			usleep(5000);

			data[0] = static_cast<unsigned char>(m_tAp471.GetAt(2, 0));
			data[1] = static_cast<unsigned char>(m_tAp471.GetAt(2, 1));

			SendJigMsg(sock, PM_IOB_DISCREATE_OUTPUT_LOW + 0x80000000, data, sizeof(data));
		}
		else
		{
			SendJigMsg(sock, PM_IOB_DISCREATE_OUTPUT_LOW + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobDiscreateOutputLow()" << endl;
}

void CJigApp::OnRecvIobDiscreateOutputHigh(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobDiscreateOutputHigh()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		// GREEN LED 조건 -> 이산신호 입력 HIGH와 다름
		if(m_tAp471.SetPort(0, 0xFF0F) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		{
			unsigned char data[2] = {0, 0};

			usleep(5000);

			data[0] = static_cast<unsigned char>(m_tAp471.GetAt(2, 0));
			data[1] = static_cast<unsigned char>(m_tAp471.GetAt(2, 1));

			SendJigMsg(sock, PM_IOB_DISCREATE_OUTPUT_HIGH + 0x80000000, data, sizeof(data));
		}
		else
		{
			SendJigMsg(sock, PM_IOB_DISCREATE_OUTPUT_HIGH + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobDiscreateOutputHigh()" << endl;
}

void CJigApp::OnRecvIobRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobRs2321()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		//if(m_tAp471.SetPort(0, 0x0000) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		if(SetIobGpioInputLow() == TRUE)
		{
			unsigned char data[3] = {0x2E, 0x11, 0x3F};

			usleep(5000);
			DebugStringHex("[write]->", data, sizeof(data));

			if(m_tTtyIobRs2321.Write(data, sizeof(data)) == -1)
			{
				// RS232 통신 오류 시, 실패 응답
				SendJigMsg(sock, PM_IOB_RS232_1 + 0x80000000, DEF_FAIL);
			}
			else
			{
				//SendJigMsg(sock, PM_IOB_RS232_1 + 0x80000000, DEF_OK);
			}
		}
		else
		{
			SendJigMsg(sock, PM_IOB_RS232_1 + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobRs2321()" << endl;
}

void CJigApp::OnRecvIobRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobRs2322()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		if(m_tAp471.SetPort(0, 0x0000) == TRUE && m_tAp471.SetPort(1, 0x0000) == TRUE && m_tAp471.SetPort(2, 0x0000) == TRUE)
		{
			unsigned char data[3] = {0x2E, 0x11, 0x3F};

			usleep(5000);
			DebugStringHex("[write]->", data, sizeof(data));

			if(m_tTtyIobRs2322.Write(data, sizeof(data)) == -1)
			{
				// RS232 통신 오류 시, 실패 응답
				SendJigMsg(sock, PM_IOB_RS232_2 + 0x80000000, DEF_FAIL);
			}
			else
			{
				//SendJigMsg(sock, PM_IOB_RS232_2 + 0x80000000, DEF_OK);
			}
		}
		else
		{
			SendJigMsg(sock, PM_IOB_RS232_2 + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobRs2322()" << endl;
}

void CJigApp::OnRecvIobRs4221(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobRs4221()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		unsigned char data[3] = {0x00, 0x01, 0x02};

		DebugStringHex("[write]->", data, sizeof(data));

		// 422 2번 포트가 GPS 포트임
		if(m_tTtyIobRs4222.Write(data, sizeof(data)) == -1)
		{
			// RS422 통신 오류 시, 실패 응답
			SendJigMsg(sock, PM_IOB_RS422_1 + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobRs4221()" << endl;
}

void CJigApp::OnRecvIobRs4222(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobRs4222()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		unsigned char data[3] = {0x2E, 0x41, 0x6F};

		DebugStringHex("[write]->", data, sizeof(data));

		m_strFindStep = "6.2 IOB_RS422_2";
		if(m_tTtyIobRs2321.Write(data, sizeof(data)) == -1)
		{
			// RS232 통신 오류 시, 실패 응답
			SendJigMsg(sock, PM_IOB_RS422_2 + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobRs4222()" << endl;
}

void CJigApp::OnRecvIobRs4223(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIobRs4223()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		unsigned char data[3] = {0x2E, 0x11, 0x3F};

		DebugStringHex("[write]->", data, sizeof(data));

		if(m_tTtyIobRs4223.Write(data, sizeof(data)) == -1)
		{
			// RS232 통신 오류 시, 실패 응답
			SendJigMsg(sock, PM_IOB_RS422_3 + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIobRs4223()" << endl;
}

void CJigApp::OnRecvIob12v(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen)
{
	cout << "++OnRecvIob12v()" << endl;

	if(CJigPkt::IsAvailablePacket(pktbuf, pktlen) == true)
	{
		unsigned char data[3] = {'S', 'V', 'E'};

		SetIobGpio(IOB_SBC1_THERMAL, 1);

		usleep(10000);

		DebugStringHex("[write]->", data, sizeof(data));

		m_strFindStep = "7.2 IOB_12V";
		if(m_tTtyS0.Write(data, sizeof(data)) == -1)
		{
			// RS232 통신 오류 시, 실패 응답
			SendJigMsg(sock, PM_IOB_12V + 0x80000000, DEF_FAIL);
		}
	}

	cout << "--OnRecvIob12v()" << endl;
}

#define MAX(A,B)	(A > B ? A : B)
void CJigApp::SelectTtyThread()
{
	cout << "START READ_TTY_THREAD!!!" << endl;

	fd_set readfs;
	int maxfd;
	unsigned int bufsize = 1024;
	unsigned char pktbuf[1024];
	//unsigned char * pktbuf = new unsigned char[bufsize];

	if(pktbuf)
	{
		maxfd = m_tTtyS0 + 1;
		maxfd = MAX(m_tTtyMccRs2321.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyMccRs2322.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyMccRs422.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyTccRs2321.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyTccRs2322.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyTccRs422.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyIobRs2321.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyIobRs2322.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyIobRs4221.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyIobRs4222.GetFd(), maxfd - 1) + 1;
		maxfd = MAX(m_tTtyIobRs4223.GetFd(), maxfd - 1) + 1;

		while(m_bRun)
		{
			//bzero(pktbuf, bufsize);

			FD_SET(m_tTtyS0, &readfs);
			FD_SET(m_tTtyMccRs2321, &readfs);
			FD_SET(m_tTtyMccRs2322, &readfs);
			FD_SET(m_tTtyMccRs422, &readfs);
			FD_SET(m_tTtyTccRs2321, &readfs);
			FD_SET(m_tTtyTccRs2322, &readfs);
			FD_SET(m_tTtyTccRs422, &readfs);
			FD_SET(m_tTtyIobRs2321, &readfs);
			FD_SET(m_tTtyIobRs2322, &readfs);
			FD_SET(m_tTtyIobRs4221, &readfs);
			FD_SET(m_tTtyIobRs4222, &readfs);
			FD_SET(m_tTtyIobRs4223, &readfs);

			select(maxfd, &readfs, NULL, NULL, NULL);

			if(FD_ISSET(m_tTtyS0, &readfs))
			{
				OnReadTtyS0(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyMccRs2321, &readfs))
			{
				OnReadTtyMccRs232_1(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyMccRs2322, &readfs))
			{
				OnReadTtyMccRs232_2(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyMccRs422, &readfs))
			{
				OnReadTtyMccRs422(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyTccRs2321, &readfs))
			{
				OnReadTtyTccRs232_1(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyTccRs2322, &readfs))
			{
				OnReadTtyTccRs232_2(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyTccRs422, &readfs))
			{
				OnReadTtyTccRs422(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyIobRs2321, &readfs))
			{
				OnReadTtyIobRs232_1(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyIobRs2322, &readfs))
			{
				OnReadTtyIobRs232_2(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyIobRs4221, &readfs))
			{
				OnReadTtyIobRs422_1(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyIobRs4222, &readfs))
			{
				OnReadTtyIobRs422_2(pktbuf, sizeof(char) * bufsize);
			}
			else if(FD_ISSET(m_tTtyIobRs4223, &readfs))
			{
				OnReadTtyIobRs422_3(pktbuf, sizeof(char) * bufsize);
			}
			else// if(FD_ISSET(m_tTtyUsb0, &readfs))
			{
				cout << "FD_ISSET() failed" << endl;
			}
		}
	}
	else
	{
	}

#if 0
	if(pktbuf)
	{
		delete [] pktbuf;
	}
	else
	{
	}
#endif

	cout << "STOP READ_TTY_THREAD!!!" << endl;
}

void CJigApp::SetMcuResponse(unsigned char * pktbuf, unsigned int pktlen)
{
	unsigned int pos1, pos2;
	string buf(const_cast<const char *>(reinterpret_cast<char *>(pktbuf)));

	for(unsigned int i = 0; i < 4; i++)
	{
		pos1 = buf.find(",", 0);
		pos2 = buf.find(",", pos1 + 1);
		m_strVolt[i] = buf.substr(pos1 + 1, pos2 - pos1 - 1);
		buf.erase(0, pos2);
	}
}

void CJigApp::OnReadTtyS0(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyS0()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	usleep(100000); //baudrate가 9600이라 적당히 기다렸다가 전체 데이터를 받도록 함.
	len = m_tTtyS0.Read(pktbuf, buflen);
	pktbuf[len] = 0; // mcu에서 받는 값은 문자열로, 마지막 값을 0으로 설정해서 사용

	if(len > 0)
	{
		// mcu에서 받는 값은 문자열임
		cout << "[read] <- " << pktbuf << endl;
		//DebugStringHex("[read] <-", pktbuf, len);

		if(m_strFindStep == "0.0 BIT")
		{
			SetMcuResponse(pktbuf, len);

			cout << "VOLT#1 = " << m_strVolt[0] << endl;
			cout << "VOLT#2 = " << m_strVolt[1] << endl;
			cout << "VOLT#3 = " << m_strVolt[2] << endl;
			cout << "VOLT#4 = " << m_strVolt[3] << endl;

			double fval;
			UINT16 ok = 0;

			fval = std::stod(m_strVolt[0]);
			if(fval >= 3.14f && fval <= 3.46f)
			{
				ok = 1;
			}

			fval = std::stod(m_strVolt[1]);
			if(ok == 1 && fval >= 3.14f && fval <= 3.46f)
			{
				ok = 2;
			}

			fval = std::stod(m_strVolt[2]);
			if(ok == 2 && fval >= 4.75f && fval <= 5.25f)
			{
				ok = 3;
			}

			fval = std::stod(m_strVolt[3]);
			if(ok == 3 && fval >= 11.4f && fval <= 12.6f)
			{
				ok = 4;
			}

			m_tAp471.SetMask(2, 8, 0);
			SetReadyLed(1);

			if(ok == 4)
			{
				SetReadyLed(0);
			}
			else
			{
				for(int i = 0; i < 5; i++)
				{
					SetReadyLed(0);
					usleep(500000);
					SetReadyLed(1);
					usleep(500000);
				}
			}

			m_tAp471.SetMask(2, 8, 1);

			m_strFindStep = "";
		}
		else if(m_strFindStep == "0.1 BIT")
		{
			SetMcuResponse(pktbuf, len);
			SendJigMsg(PM_BIT1 + 0x80000000, reinterpret_cast<unsigned char *>(const_cast<char *>(m_strVolt[0].c_str())), m_strVolt[0].length());
			m_strFindStep = "";
		}
		else if(m_strFindStep == "0.2 BIT")
		{
			SetMcuResponse(pktbuf, len);
			SendJigMsg(PM_BIT2 + 0x80000000, reinterpret_cast<unsigned char *>(const_cast<char *>(m_strVolt[1].c_str())), m_strVolt[1].length());
			m_strFindStep = "";
		}
		else if(m_strFindStep == "0.3 BIT")
		{
			SetMcuResponse(pktbuf, len);
			SendJigMsg(PM_BIT3 + 0x80000000, reinterpret_cast<unsigned char *>(const_cast<char *>(m_strVolt[2].c_str())), m_strVolt[2].length());
			m_strFindStep = "";
		}
		else if(m_strFindStep == "0.4 BIT")
		{
			SetMcuResponse(pktbuf, len);
			SendJigMsg(PM_BIT4 + 0x80000000, reinterpret_cast<unsigned char *>(const_cast<char *>(m_strVolt[3].c_str())), m_strVolt[3].length());
			m_strFindStep = "";
		}
		else if(m_strFindStep == "7.2 IOB_12V")
		{
			SetIobGpio(IOB_SBC1_THERMAL, 0);
			SendJigMsg(PM_IOB_12V + 0x80000000, pktbuf, len);
		}
		else
		{
		}
	}
	else
	{
	}

	cout << "--OnReadTtyS0()" << endl;
}

void CJigApp::OnReadTtyMccRs232_1(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyMccRs232_1()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyMccRs2321.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_MCC_RS232_1 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyMccRs232_1()" << endl;
}

void CJigApp::OnReadTtyMccRs232_2(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyMccRs232_2()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyMccRs2322.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_MCC_RS232_2 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyMccRs232_2()" << endl;
}

void CJigApp::OnReadTtyOccRs232_1(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyOccRs232_1()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyOccRs2321.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_OCC_RS232_1 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyOccRs232_1()" << endl;
}

void CJigApp::OnReadTtyOccRs232_2(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyOccRs232_2()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyOccRs2322.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_OCC_RS232_2 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyOccRs232_2()" << endl;
}

void CJigApp::OnReadTtyTccRs232_1(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyTccRs232_1()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyTccRs2321.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_TCC_RS232_1 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyTccRs232_1()" << endl;
}

void CJigApp::OnReadTtyTccRs232_2(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyTccRs232_2()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyTccRs2322.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_TCC_RS232_2 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyTccRs232_2()" << endl;
}

void CJigApp::OnReadTtyIobRs232_1(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyIobRs232_1()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyIobRs2321.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		if(m_strFindStep == "6.2 IOB_RS422_2")
		{
			if(pktbuf[0] == 0x1E && pktbuf[1] == 0x42 && pktbuf[2] == 0x00 && pktbuf[3] == 0x3F && pktbuf[4] == 0x9F)
			{
				m_strFindStep = "6.4 IOB_RS422_2";

				unsigned char data[3] = {0x2E, 0x11, 0x3F};

				DebugStringHex("[write]->", data, sizeof(data));
				m_tTtyIobRs4222.Write(data, sizeof(data));
			}
			else
			{
				SendJigMsg(PM_IOB_RS422_2 + 0x80000000, DEF_FAIL);
			}
		}
		else if(m_strFindStep == "6.6 IOB_RS422_2")
		{
			if(pktbuf[0] == 0x1E && pktbuf[1] == 0x52 && pktbuf[2] == 0x00 && pktbuf[3] == 0x1F && pktbuf[4] == 0x8F)
			{
				m_strFindStep = "6.8 IOB_RS422_2";

				unsigned char data[3] = {0x2E, 0x11, 0x3F};

				DebugStringHex("[write]->", data, sizeof(data));
				m_tTtyIobRs4222.Write(data, sizeof(data));
			}
			else
			{
				SendJigMsg(PM_IOB_RS422_2 + 0x80000000, DEF_FAIL);
			}
		}
		else
		{
			SendJigMsg(PM_IOB_RS232_1 + 0x80000000, pktbuf, len);
		}
	}

	cout << "--OnReadTtyIobRs232_1()" << endl;
}

void CJigApp::OnReadTtyIobRs232_2(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyIobRs232_2()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyIobRs2322.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		SendJigMsg(PM_IOB_RS232_2 + 0x80000000, pktbuf, len);
	}

	cout << "--OnReadTtyIobRs232_2()" << endl;
}

void CJigApp::OnReadTtyMccRs422(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyMccRs422()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyMccRs422.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_MCC_RS422 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyMccRs422()" << endl;
}

void CJigApp::OnReadTtyOccRs422(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyOccRs422()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyOccRs422.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_OCC_RS422 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyOccRs422()" << endl;
}

void CJigApp::OnReadTtyTccRs422(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyTccRs422()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyTccRs422.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		CJigPkt pkt;

		pkt.Create(PM_TCC_RS422 + 0x80000000, len, pktbuf);

		SendPacket(pkt.buf(), pkt.length());
	}
	else
	{
	}

	cout << "--OnReadTtyTccRs422()" << endl;
}

void CJigApp::OnReadTtyIobRs422_1(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyIobRs422_1()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyIobRs4221.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		if(m_strFindStep == "6.8 IOB_RS422_2")
		{
			if(pktbuf[0] == 0x2E && pktbuf[1] == 0x11 && pktbuf[2] == 0x3F)
			{
				SendJigMsg(PM_IOB_RS422_2 + 0x80000000, DEF_OK);
			}
			else
			{
				SendJigMsg(PM_IOB_RS422_2 + 0x80000000, DEF_FAIL);
			}
		}
		else
		{
			SendJigMsg(PM_IOB_RS422_1 + 0x80000000, pktbuf, len);
		}
	}
	else
	{
	}

	cout << "--OnReadTtyIobRs422_1()" << endl;
}

void CJigApp::OnReadTtyIobRs422_2(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyIobRs422_2()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyIobRs4222.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		if(m_strFindStep == "6.4 IOB_RS422_2")
		{
			if(pktbuf[0] == 0x2E && pktbuf[1] == 0x11 && pktbuf[2] == 0x3F)
			{
				m_strFindStep = "6.6 IOB_RS422_2";

				unsigned char data[3] = {0x2E, 0x51, 0x7F};

				DebugStringHex("[write]->", data, sizeof(data));
				m_tTtyIobRs2321.Write(data, sizeof(data));
			}
			else
			{
				SendJigMsg(PM_IOB_RS422_2 + 0x80000000, DEF_FAIL);
			}
		}
		else
		{
			SendJigMsg(PM_IOB_RS422_2 + 0x80000000, pktbuf, len);
		}
	}
	else
	{
	}

	cout << "--OnReadTtyIobRs422_2()" << endl;
}

void CJigApp::OnReadTtyIobRs422_3(unsigned char * pktbuf, unsigned int buflen)
{
	cout << "++OnReadTtyIobRs422_3()" << endl;

	int len;
	unsigned int nReadMsgID;
	bool bExtract = false;

	len = m_tTtyIobRs4223.Read(pktbuf, buflen);

	if(len > 0)
	{
		DebugStringHex("[read] <-", pktbuf, len);

		SendJigMsg(PM_IOB_RS422_3 + 0x80000000, pktbuf, len);
	}
	else
	{
	}

	cout << "--OnReadTtyIobRs422_3()" << endl;
}

void CJigApp::SetGpio(UINT32 pin_name, UINT16 onoff)
{
	UINT32 port = pin_name / 100;
	UINT32 pin = pin_name % 100;
	UINT16 val;

	if(port <= 2)
	{
		if(pin > 15)
		{
			pin = 0;
		}

		val = m_tAp471.SetAt(port, pin, onoff);
	}
}

void CJigApp::SetOccGpio(UINT32 pin_name, UINT16 onoff)
{
	if(pin_name >= OCC_GPIO1 && pin_name <= OCC_GPIO4)
	{
		SetGpio(pin_name, onoff);
	}
}

void CJigApp::SetTccGpio(UINT32 pin_name, UINT16 onoff)
{
	if(pin_name >= TCC_GPIO1 && pin_name <= TCC_GPIO4)
	{
		SetGpio(pin_name, onoff);
	}
}

void CJigApp::SetIobGpio(UINT32 pin_name, UINT16 onoff)
{
	if(pin_name >= IOB_SBC1_CARDFAIL && pin_name <= IOB_STATUS_RED)
	{
		SetGpio(pin_name, onoff);
	}
}

void CJigApp::SetReadyLed(UINT16 onoff)
{
	SetGpio(JIG_READY, onoff);
}

UINT16 CJigApp::GetReadyLed()
{
	return GetGpio(JIG_READY);
}

UINT16 CJigApp::GetGpio(UINT32 pin_name)
{
	UINT32 port = pin_name / 100;
	UINT32 pin = pin_name % 100;
	UINT16 val = 0;

	if(port <= 2)
	{
		if(pin > 15)
		{
			pin = 0;
		}

		val = m_tAp471.GetAt(port, pin);
	}

	return val;
}

UINT16 CJigApp::GetOccGpio(UINT32 pin_name)
{
	UINT16 ret = 0;

	if(pin_name >= OCC_GPIO1 && pin_name <= OCC_GPIO4)
	{
		ret = GetGpio(pin_name);
	}

	return ret;
}

UINT16 CJigApp::GetTccGpio(UINT32 pin_name)
{
	UINT16 ret = 0;

	if(pin_name >= TCC_GPIO1 && pin_name <= TCC_GPIO4)
	{
		ret = GetGpio(pin_name);
	}

	return ret;
}

UINT16 CJigApp::GetIobGpio(UINT32 pin_name)
{
	UINT16 ret = 0;

	if(pin_name >= IOB_SBC1_CARDFAIL && pin_name <= IOB_STATUS_RED)
	{
		ret = GetGpio(pin_name);
	}

	return ret;
}

