#pragma once
#include "commpkt.h"

/*

[START CODE]		[MSG ID]			[Data길이]			[D[0]~D[N]]
[4 byte]			[4 bytes(UINT)]		[4 bytes(UINT)]		[N bytes(BYTE값 배열)]
[0xFFAF0000]		[명령어]			[Data길이]			[데이터]

*/

enum _PKT_VAL
{
	PKT_DATA_OFFSET = 12,
	START_CODE = 0xFFAF0000,
};

enum _PKT_MSG
{
	PM_END = 0,

	PM_MCC_RS232_1 = 1,
	PM_MCC_RS232_2,
	PM_MCC_RS422,
	PM_MCC_GPIO_INPUT_READY,
	PM_MCC_GPIO_INPUT,
	PM_OCC_RS232_1,
	PM_OCC_RS232_2,
	PM_OCC_RS422,
	PM_OCC_GPIO_INPUT_READY,
	PM_OCC_GPIO_INPUT,
	PM_OCC_GPIO_1_READY,
	PM_OCC_GPIO_1,
	PM_OCC_GPIO_2_READY,
	PM_OCC_GPIO_2,
	PM_OCC_GPIO_3_READY,
	PM_OCC_GPIO_3,
	PM_OCC_GPIO_4_READY,
	PM_OCC_GPIO_4,
	PM_TCC_RS232_1,
	PM_TCC_RS232_2,
	PM_TCC_RS422,
	PM_TCC_ID_READY,
	PM_TCC_ID,
	PM_TCC_BIT_READY,
	PM_TCC_BIT,
	PM_TCC_THERMAL_READY,
	PM_TCC_THERMAL,
	PM_TCC_PCIE,
	PM_IOB_DISCREATE_INPUT_HIGH,
	PM_IOB_DISCREATE_INPUT_LOW,
	PM_IOB_DISCREATE_OUTPUT_HIGH,
	PM_IOB_DISCREATE_OUTPUT_LOW,
	PM_IOB_RS232_1,
	PM_IOB_RS232_2,
	PM_IOB_RS422_1,
	PM_IOB_RS422_2,
	PM_IOB_RS422_3,
	PM_IOB_12V,
//	PM_IOB_BIT_READY,
//	PM_IOB_BIT,
//	PM_IOB_ID_READY,
//	PM_IOB_ID,
//	PM_IOB_THERMAL_READY,
//	PM_IOB_THERMAL,
//	PM_IOB_FANSENSOR_READY,
//	PM_IOB_FANSENSOR,
//	PM_IOB_LED_READY,
//	PM_IOB_LED,
//	PM_IOB_12V_READY,
//	PM_IOB_12V,
	PM_IOB_END,
	PM_BIT1 = 5000,
	PM_BIT2,
	PM_BIT3,
	PM_BIT4,
	PM_SHUTDOWN,
};

class CJigPkt : public CCommPkt
{
public:
	CJigPkt();
	~CJigPkt();

public:
	unsigned int Create(unsigned int msgid, unsigned int datalen = 0, unsigned char * data = 0);

public:
	void SetMsgID(unsigned int msgid);
	unsigned int GetMsgID();
	void SetDataLength(unsigned int datalen);
	unsigned int GetDataLength();
	void SetDataVal(unsigned char data, unsigned int data_no = 0);
	unsigned char GetDataVal(unsigned int data_no = 0);
	unsigned char * GetData();

public:
	static bool IsAvailablePacket(unsigned char * pktbuf, unsigned int pktlen);
	static unsigned int ExtractMsgID(unsigned char * pktbuf, unsigned int pktlen, bool * extract);
	static unsigned int ExtractDataLength(unsigned char * pktbuf, unsigned int pktlen, bool * extract);
	static unsigned char ExtractData8(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no = 0);
	static unsigned short ExtractData16(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no = 0);
	static unsigned int ExtractData32(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no = 0);
	static unsigned long long ExtractData64(unsigned char * pktbuf, unsigned int pktlen, bool * extract, unsigned int data_no);
	static unsigned char * ExtractData(unsigned char * pktbuf, unsigned int pktlen, bool * extract);
};

