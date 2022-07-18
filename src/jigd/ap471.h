#pragma once

#include "apptype.h"
#include "./apcommon/apcommon.h"
#include "./AP471/AP471.h"
#include "thread.h"

class CAp471
{
public:
	CAp471();
	~CAp471();

protected:
	CThread m_tInterruptThread;
	cblk471 m_tCblk;

public:
	bool Open();
	void Close();
	UINT16 GetAt(UINT16 port, UINT16 point);
	UINT16 GetPort(UINT16 port);
	BOOL SetAt(UINT16 port, UINT16 point, UINT16 val);
	BOOL SetPort(UINT16 port, UINT16 val);
	BOOL SetMask(UINT16 port, UINT16 val);
	BOOL SetMask(UINT16 port, UINT16 point, UINT16 val);

	void InterruptThread();

	operator BOOL () const
	{
		return m_tCblk.bAP;
	};
};

