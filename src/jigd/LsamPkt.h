#pragma once

#include "commpkt.h"
#include "apptype.h"
#include "ap471.h"
#include <stdlib.h>

/*
//임시
[START CODE]		[MSG ID]			[Data길이]					[D[0]~D[N]]
[4 bytes]			[4 bytes(UINT)]		[4 bytes(UINT)]				[N bytes(BYTE값 배열)]
[0xAAFA0000]		[명령어]				[Data길이]					[데이터]


//IOB PACKET
[SOF]				[MSGID]				[D[0]~D[N]]					[checksum]
[1 byte]			[1 bytes]			[N bytes(BYTE값 배열)]		[1 bytes]
[0x2E]				[명령어]				[데이터]						[SOF ~ 데이터]

*/

enum _LSAM_PKT_VAL
{
	LPKT_DATA_OFFSET = 12,
	LPKT_START_CODE = 0xAAFA0000,
};

enum _IOB_PKT_VAL
{
	IOB_VAL_DATA_OFFSET = 2,
	IOB_VAL_SOF_REQ = 0x1E,
	IOB_VAL_SOF_RES = 0x2E,
};

enum _IOB_PKT_MSG
{
	IOB_CBIT_REQ = 0x11,
	IOB_CBIT_RES,
};

enum _LSAM_PKT_MSG
{
	LPM_TTY = 0,
	LPM_MCC_RS232_1 = 0,
	LPM_MCC_RS232_2,
	LPM_MCC_RS422,
	LPM_MCC_GPIO_INPUT_READY,
	LPM_MCC_GPIO_INPUT,
	LPM_OCC_RS232_1,
	LPM_OCC_RS232_2,
	LPM_OCC_RS422,
	LPM_OCC_GPIO_INPUT_READY,
	LPM_OCC_GPIO_INPUT,
	LPM_TCC_RS232_1,
	LPM_TCC_RS232_2,
	LPM_TCC_RS422,
	LPM_TCC_ID_READY,
	LPM_TCC_ID,
	LPM_TCC_BIT_READY,
	LPM_TCC_BIT,
	LPM_TCC_THERMAL_READY,
	LPM_TCC_THERMAL,
	LPM_TCC_PCIE,
	//LPM_IOB_RS232_1,
	//LPM_IOB_RS232_2,
	//LPM_IOB_RS422_1,
	//LPM_IOB_RS422_2,
	//LPM_IOB_BIT_READY,
	//LPM_IOB_BIT,
	//LPM_IOB_ID_READY,
	//LPM_IOB_ID,
	//LPM_IOB_THERMAL_READY,
	//LPM_IOB_THERMAL,
	//LPM_IOB_FANSENSOR_READY,
	//LPM_IOB_FANSENSOR,
	//LPM_IOB_LED_READY,
	//LPM_IOB_LED,
	//LPM_IOB_12V_READY,
	//LPM_IOB_12V,
};

class CLsamPkt :
	public CCommPkt
{
public:
	CLsamPkt(void);
	~CLsamPkt(void);

public:
	UINT Create(UINT nMsgID, UINT nDataLen = 0, BYTE * pData = NULL);

public:
	void SetMsgID(UINT nMsgID);
	UINT GetMsgID();
	void SetDataLength(UINT nDataLen);
	UINT GetDataLength();
	void SetDataVal(BYTE cData, UINT nDataNo = 0);
	BYTE GetDataVal(UINT nDataNo = 0);
	BYTE * GetData();

//	static extract 메소드들
	static BOOL IsAvailablePacket(BYTE * pPacket, UINT nPacketLen);
	static UINT ExtractMsgID(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
	static UINT ExtractDataLength(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
	static BYTE ExtractData8(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static USHORT ExtractData16(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static UINT ExtractData32(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static ULONGLONG ExtractData64(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static BYTE * ExtractData(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
};


class CIob232Pkt :
	public CCommPkt
{
public:
	CIob232Pkt(void);
	~CIob232Pkt(void);

protected:
	UINT m_nDataLen;

public:
	UINT Create(BYTE nMsgID, UINT nDataLen = 0, BYTE * pData = NULL);

public:
	void SetMsgID(BYTE nMsgID);
	BYTE GetMsgID();
	void SetDataVal(BYTE cData, UINT nDataNo = 0);
	BYTE GetDataVal(UINT nDataNo = 0);
	BYTE * GetData();
	void SetData(BYTE * pData, UINT nDataLen);
	void SetChecksum();
	BYTE GetChecksum();

// static extract 메소드들
	static BOOL IsAvailablePacket(BYTE * pPacket, UINT nPacketLen);
	static BYTE ExtractMsgID(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
	static BYTE ExtractData8(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static USHORT ExtractData16(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static UINT ExtractData32(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static ULONGLONG ExtractData64(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo = 0);
	static BYTE * ExtractData(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
	static BYTE ExtractChecksum(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract);
};

