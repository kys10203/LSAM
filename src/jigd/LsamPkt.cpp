#include "LsamPkt.h"
#include <arpa/inet.h>

CLsamPkt::CLsamPkt(void)
{
}


CLsamPkt::~CLsamPkt(void)
{
}


UINT CLsamPkt::Create(UINT nMsgID, UINT nDataLen, BYTE * pData)
{
	UINT nRet = 0;

	if(m_pPktBuf == NULL)
	{
		nRet = CCommPkt::Create(nDataLen + LPKT_DATA_OFFSET);

		if(nRet != 0)
		{
			*((UINT *)(m_pPktBuf + 0)) = htonl(LPKT_START_CODE);
			*((UINT *)(m_pPktBuf + 4)) = htonl(nMsgID);
			*((UINT *)(m_pPktBuf + 8)) = htonl(nDataLen);

			if(nDataLen > 0 && pData != NULL)
			{
				memcpy(m_pPktBuf + LPKT_DATA_OFFSET, pData, nDataLen);
			}
		}
	}

	return nRet;
}

void CLsamPkt::SetMsgID(UINT nMsgID)
{
	if(m_pPktBuf)
	{
		*((UINT *)(m_pPktBuf + 4)) = htonl(nMsgID);
	}
}

UINT CLsamPkt::GetMsgID()
{
	UINT nRet = 0;

	if(m_pPktBuf)
	{
		nRet = *((UINT *)(m_pPktBuf + 4));
		nRet = htonl(nRet);
	}

	return nRet;
}

void CLsamPkt::SetDataLength(UINT nDataLen)
{
	if(m_pPktBuf)
	{
		*((UINT *)(m_pPktBuf + 8)) = htonl(nDataLen);
	}
}

UINT CLsamPkt::GetDataLength()
{
	UINT nRet = 0;

	if(m_pPktBuf)
	{
		nRet = *((UINT *)(m_pPktBuf + 8));
		nRet = htonl(nRet);
	}

	return nRet;
}

void CLsamPkt::SetDataVal(BYTE cData, UINT nDataNo)
{
	if((m_pPktBuf) && (m_nPktLen >= (LPKT_DATA_OFFSET + nDataNo + 1)))
	{
		m_pPktBuf[LPKT_DATA_OFFSET + nDataNo] = cData;
	}
}

BYTE CLsamPkt::GetDataVal(UINT nDataNo)
{
	BYTE cRet = 0;

	if(m_pPktBuf)
	{
		cRet = m_pPktBuf[LPKT_DATA_OFFSET + nDataNo];
	}

	return cRet;
}

BYTE * CLsamPkt::GetData()
{
	BYTE * pRet = 0;

	if(m_pPktBuf)
	{
		pRet = m_pPktBuf + LPKT_DATA_OFFSET;
	}

	return pRet;
}

// static extract 메소드들
BOOL CLsamPkt::IsAvailablePacket(BYTE * pPacket, UINT nPacketLen)
{
	BOOL bRet = FALSE;

	if(pPacket)
	{
		// 시작코드가 일치하는 지 비교

		UINT sc = 0;

		sc = *((UINT *)(pPacket + 0));
		sc = ntohl(sc);

		if(sc == LPKT_START_CODE)
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
		}
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

UINT CLsamPkt::ExtractMsgID(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	UINT nRet = 0;

	if(pPacket)
	{
		nRet = *((UINT *)(pPacket + 4));
		nRet = ntohl(nRet);
		*pExtract = TRUE;
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

UINT CLsamPkt::ExtractDataLength(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	UINT nRet = 0;

	if(pPacket)
	{
		nRet = *((UINT *)(pPacket + 8));
		nRet = ntohl(nRet);
		*pExtract= TRUE;
	}
	else
	{
		*pExtract= FALSE;
	}

	return nRet;
}

BYTE CLsamPkt::ExtractData8(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	BYTE cRet = 0;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - LPKT_DATA_OFFSET)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			cRet = pPacket[LPKT_DATA_OFFSET + nDataNo];
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return cRet;
}

USHORT CLsamPkt::ExtractData16(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	USHORT nRet = 0;
	USHORT * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - LPKT_DATA_OFFSET) / sizeof(USHORT)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<USHORT *>(pPacket + LPKT_DATA_OFFSET + nDataNo * sizeof(USHORT));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

UINT CLsamPkt::ExtractData32(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	UINT nRet = 0;
	UINT * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - LPKT_DATA_OFFSET) / sizeof(UINT)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<UINT *>(pPacket + LPKT_DATA_OFFSET + nDataNo * sizeof(UINT));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

ULONGLONG CLsamPkt::ExtractData64(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	ULONGLONG nRet = 0;
	ULONGLONG * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - LPKT_DATA_OFFSET) / sizeof(ULONGLONG)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<ULONGLONG *>(pPacket + LPKT_DATA_OFFSET + nDataNo * sizeof(ULONGLONG));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

BYTE * CLsamPkt::ExtractData(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	BYTE * pRet = 0;
	BOOL bErr = FALSE;

	bErr = IsAvailablePacket(pPacket, nPacketLen);
	if(bErr)
	{
		pRet = pPacket + LPKT_DATA_OFFSET;
		*pExtract = TRUE;
	}
	else
	{
		*pExtract = FALSE;
	}

	return pRet;
}



CIob232Pkt::CIob232Pkt(void)
{
}


CIob232Pkt::~CIob232Pkt(void)
{
}


UINT CIob232Pkt::Create(BYTE nMsgID, UINT nDataLen, BYTE * pData)
{
	UINT nRet = 0;

	if(m_pPktBuf == NULL)
	{
		nRet = CCommPkt::Create(nDataLen + IOB_VAL_DATA_OFFSET + 1);

		if(nRet != 0)
		{
			*((BYTE *)(m_pPktBuf + 0)) = IOB_VAL_SOF_REQ;
			*((BYTE *)(m_pPktBuf + 1)) = nMsgID;

			if(nDataLen > 0 && pData != NULL)
			{
				m_nDataLen = nDataLen;
				memcpy(m_pPktBuf + IOB_VAL_DATA_OFFSET, pData, nDataLen);
			}
		}
	}

	return nRet;
}

void CIob232Pkt::SetMsgID(BYTE nMsgID)
{
	if(m_pPktBuf)
	{
		*((BYTE *)(m_pPktBuf + 1)) = nMsgID;
	}
}

BYTE CIob232Pkt::GetMsgID()
{
	BYTE nRet = 0;

	if(m_pPktBuf)
	{
		nRet = *((BYTE *)(m_pPktBuf + 1));
	}

	return nRet;
}

void CIob232Pkt::SetDataVal(BYTE cData, UINT nDataNo)
{
	if((m_pPktBuf) && (m_nPktLen >= (IOB_VAL_DATA_OFFSET + nDataNo + 1)))
	{
		m_pPktBuf[IOB_VAL_DATA_OFFSET + nDataNo] = cData;
	}
}

BYTE CIob232Pkt::GetDataVal(UINT nDataNo)
{
	BYTE cRet = 0;

	if(m_pPktBuf)
	{
		cRet = m_pPktBuf[IOB_VAL_DATA_OFFSET + nDataNo];
	}

	return cRet;
}

BYTE * CIob232Pkt::GetData()
{
	BYTE * pRet = 0;

	if(m_pPktBuf)
	{
		pRet = m_pPktBuf + IOB_VAL_DATA_OFFSET;
	}

	return pRet;
}

void CIob232Pkt::SetChecksum()
{
	if(m_pPktBuf)
	{
		m_pPktBuf[m_nDataLen + IOB_VAL_DATA_OFFSET] = IOB_VAL_SOF_REQ + GetMsgID();
	}
}

BYTE CIob232Pkt::GetChecksum()
{
	BYTE cRet = 0;

	if(m_pPktBuf)
	{
		cRet = m_pPktBuf[m_nDataLen + IOB_VAL_DATA_OFFSET];
	}

	return cRet;
}

// static extract 메소드들
BOOL CIob232Pkt::IsAvailablePacket(BYTE * pPacket, UINT nPacketLen)
{
	BOOL bRet = FALSE;

	if(pPacket)
	{
		// 시작코드가 일치하는 지 비교

		BYTE sc = *(pPacket + 0), cs = *(pPacket + nPacketLen - 1);
		BYTE csval = 0;

		for(UINT i = 0; i < nPacketLen - 1; i++)
		{
			csval += *(pPacket + i);
		}

		if(sc == IOB_VAL_SOF_REQ && cs == csval)
		{
			bRet = TRUE;
		}
		else
		{
			if(sc == IOB_VAL_SOF_RES && cs == csval)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BYTE CIob232Pkt::ExtractMsgID(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	BYTE nRet = 0;

	if(pPacket)
	{
		nRet = *(pPacket + 1);
		*pExtract = TRUE;
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

BYTE CIob232Pkt::ExtractData8(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	BYTE cRet = 0;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - IOB_VAL_DATA_OFFSET)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			cRet = pPacket[IOB_VAL_DATA_OFFSET + nDataNo];
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return cRet;
}

USHORT CIob232Pkt::ExtractData16(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	USHORT nRet = 0;
	USHORT * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - IOB_VAL_DATA_OFFSET) / sizeof(USHORT)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<USHORT *>(pPacket + IOB_VAL_DATA_OFFSET + nDataNo * sizeof(USHORT));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

UINT CIob232Pkt::ExtractData32(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	UINT nRet = 0;
	UINT * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - IOB_VAL_DATA_OFFSET) / sizeof(UINT)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<UINT *>(pPacket + IOB_VAL_DATA_OFFSET + nDataNo * sizeof(UINT));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

ULONGLONG CIob232Pkt::ExtractData64(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract, UINT nDataNo)
{
	ULONGLONG nRet = 0;
	ULONGLONG * pData;
	BOOL bErr = FALSE;

	if(pPacket && (nDataNo < (nPacketLen - IOB_VAL_DATA_OFFSET) / sizeof(ULONGLONG)))
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			pData = reinterpret_cast<ULONGLONG *>(pPacket + IOB_VAL_DATA_OFFSET + nDataNo * sizeof(ULONGLONG));
			nRet = *pData;
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return nRet;
}

BYTE * CIob232Pkt::ExtractData(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	BYTE * pRet = 0;
	BOOL bErr = FALSE;

	bErr = IsAvailablePacket(pPacket, nPacketLen);
	if(bErr)
	{
		pRet = pPacket + IOB_VAL_DATA_OFFSET;
		*pExtract = TRUE;
	}
	else
	{
		*pExtract = FALSE;
	}

	return pRet;
}

BYTE CIob232Pkt::ExtractChecksum(BYTE * pPacket, UINT nPacketLen, BOOL * pExtract)
{
	BYTE cRet = 0;
	BOOL bErr = FALSE;

	if(pPacket)
	{
		bErr = IsAvailablePacket(pPacket, nPacketLen);
		if(bErr)
		{
			cRet = pPacket[nPacketLen - 1];
			*pExtract = TRUE;
		}
		else
		{
			*pExtract = FALSE;
		}
	}
	else
	{
		*pExtract = FALSE;
	}

	return cRet;
}
