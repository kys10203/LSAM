#pragma once

#include "apptype.h"
#include "appdef.h"
#include "sock.h"
#include <string>
#include <vector>
#include "thread.h"
#include "tty.h"
#include "ap471.h"
#include <string>

using namespace std;

enum
{
	// define
	DEF_FAIL = 0,
	DEF_OK = 1,

	// PORT 0
	IOB_SBC1_CARDFAIL = 0,
	IOB_SBC2_CARDFAIL,
	IOB_SBC3_CARDFAIL,
	IOB_SBC4_CARDFAIL,
	IOB_SBC1_THERMAL,
	IOB_SBC2_THERMAL,
	IOB_SBC3_THERMAL,
	IOB_SBC4_THERMAL,
	IOB_28V_BIT1,
	IOB_28V_BIT2,
	IOB_12V_BIT1,
	IOB_12V_BIT2,
	IOB_5V_BIT1,
	IOB_5V_BIT2,
	IOB_33V_BIT1,
	IOB_33V_BIT2,

	// PORT 1
	IOB_FAN1_SENSOR = 100,
	IOB_FAN2_SENSOR,
	IOB_FAN3_SENSOR,
	IOB_FAN4_SENSOR,
	IOB_ID,

	TCC_GPIO1 = 108,
	TCC_GPIO2,
	TCC_GPIO3,
	TCC_GPIO4,

	// PORT 2
	IOB_STATUS_GREEN = 200,
	IOB_STATUS_RED,

	OCC_GPIO1 = 204,
	OCC_GPIO2,
	OCC_GPIO3,
	OCC_GPIO4,

	JIG_READY,
};

class CJigApp
{
public:
	CJigApp();
	~CJigApp();

protected:
	bool m_bRun;
	int m_nLastAcptSock;
	CSocket m_tSocket;
	string m_strFindStep;

	CAp471 m_tAp471;

	CTty m_tTtyS0;//MCU로부터 전원데이터 확인
	CTty m_tTtyMccRs2321;
	CTty m_tTtyMccRs2322;
	CTty m_tTtyMccRs422;
	CTty m_tTtyOccRs2321;
	CTty m_tTtyOccRs2322;
	CTty m_tTtyOccRs422;
	CTty m_tTtyTccRs2321;
	CTty m_tTtyTccRs2322;
	CTty m_tTtyTccRs422;
	CTty m_tTtyIobRs2321;
	CTty m_tTtyIobRs2322;
	CTty m_tTtyIobRs4221;
	CTty m_tTtyIobRs4222;
	CTty m_tTtyIobRs4223;

	string m_strVolt[4];

public:
	void Init();
	
public:
	bool Run();
	void AcceptThread();
	void RecvThread();

public:
	void DbgThread();
	void DebugStringHex(string chkmsg, unsigned char * buf, int len);

public:
	void SelectTtyThread();

protected:
	void SetGpio(UINT32 pin_name, UINT16 onoff);
	UINT16 GetGpio(UINT32 pin_name); 

public:
	void SetOccGpio(UINT32 pin_name, UINT16 onoff);
	void SetTccGpio(UINT32 pin_name, UINT16 onoff);
	void SetIobGpio(UINT32 pin_name, UINT16 onoff);
	UINT16 GetOccGpio(UINT32 pin_name); 
	UINT16 GetTccGpio(UINT32 pin_name); 
	UINT16 GetIobGpio(UINT32 pin_name); 

public:
	void SetReadyLed(UINT16 onoff);
	UINT16 GetReadyLed();

public:
	void OnReadTtyS0(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyMccRs232_1(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyMccRs232_2(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyOccRs232_1(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyOccRs232_2(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyTccRs232_1(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyTccRs232_2(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyIobRs232_1(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyIobRs232_2(unsigned char * pktbuf, unsigned int pktlen);

	void OnReadTtyMccRs422(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyOccRs422(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyTccRs422(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyIobRs422_1(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyIobRs422_2(unsigned char * pktbuf, unsigned int pktlen);
	void OnReadTtyIobRs422_3(unsigned char * pktbuf, unsigned int pktlen);

public:
	void SendPacket(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void SendPacket(unsigned char * pktbuf, unsigned int pktlen);
	void SendJigMsg(CTcpSocket * sock, unsigned int msgid, unsigned char * pktbuf, unsigned int pktlen);
	void SendJigMsg(CTcpSocket * sock, unsigned int msgid, unsigned char ok);
	void SendJigMsg(unsigned int msgid, unsigned char * pktbuf, unsigned int pktlen);
	void SendJigMsg(unsigned int msgid, unsigned char ok);

public:
	void OnRecvBit1(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvBit2(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvBit3(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvBit4(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

	void OnRecvMccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvMccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvMccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvMccGpioInputReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvMccGpioInput(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

	void OnRecvOccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpioInputReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpioInput(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio1Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio1(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio2Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio2(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio3Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio3(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio4Ready(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvOccGpio4(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

	void OnRecvTccRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccRs422(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccIdReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccId(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccBitReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccBit(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccThermalReady(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccThermal(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvTccPcie(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

	void OnRecvIobDiscreateInputHigh(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvIobDiscreateInputLow(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvIobDiscreateOutputLow(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvIobDiscreateOutputHigh(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvIobRs2321(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);
	void OnRecvIobRs2322(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

	void OnRecvIobRs4221(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);//GPS
	void OnRecvIobRs4222(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);//SBC1
	void OnRecvIobRs4223(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);//SBC3
	void OnRecvIob12v(CTcpSocket * sock, unsigned char * pktbuf, unsigned int pktlen);

public:
	void RunBit();
	void PluseThread();
	bool m_bPluseStop;
	BOOL SetIobGpioInputHigh();
	BOOL SetIobGpioInputLow();
	void SetMcuResponse(unsigned char * pktbuf, unsigned int pktlen);
};

