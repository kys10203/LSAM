#include "ap471.h"
#include "thread.h"
#include <errno.h>
#include <iostream>

using namespace std;

void * interrupt_thread(void * arg)
{
	if(arg != NULL)
	{
		CAp471 * ap = reinterpret_cast<CAp471 *>(arg);

		ap->InterruptThread();
	}

	return 0;
}

CAp471::CAp471()
{
	memset(&m_tCblk, 0, sizeof(m_tCblk));
}

CAp471::~CAp471()
{
	Close();
}

bool CAp471::Open()
{
	bool ret = false;
	struct sblk471 s_block471;
	m_tCblk.bAP = FALSE;
	m_tCblk.bInitialized = FALSE;
	m_tCblk.sblk_ptr = (struct sblk471 *)&s_block471;

	if(InitAPLib() != S_OK)
	{
		perror("InitAPLib() FAILED");
		ret = false;
	}
	else
	{
		int ap_instance = 0;

		if(APOpen(ap_instance, &m_tCblk.nHandle, const_cast<char *>(DEVICE_NAME)) != S_OK)
		{
			perror("APOpen() FAILED");
			ret = false;
		}
		else
		{
			if(APInitialize(m_tCblk.nHandle) == S_OK)
			{
				long addr = 0;

				GetAPAddress(m_tCblk.nHandle, &addr);

				m_tCblk.brd_ptr = reinterpret_cast<struct map471 *>(addr);
				m_tCblk.bInitialized = TRUE;
				m_tCblk.bAP = TRUE;

				if(m_tInterruptThread.Create(interrupt_thread, this) == 0)
				{
					ret = true;
				}
				else
				{
					perror("pthread_create() FAILED");
					ret = false;
				}
			}
			else
			{
				perror("APInitialize() FAILED");
				ret = false;
			}
		}
	}

    return ret;
}

void CAp471::Close()
{
	if(m_tCblk.bAP)
	{
		DisableAPInterrupts(m_tCblk.nHandle);
		APTerminateBlockedStart(m_tCblk.nHandle);
		//phtread_join
		APClose(m_tCblk.nHandle);
		memset(&m_tCblk, 0, sizeof(m_tCblk));

		usleep(100000);
	}
}

UINT16 CAp471::GetAt(UINT16 port, UINT16 point)
{
	UINT16 ret = 0;

	if(m_tCblk.bInitialized)
	{
		if(port > 2)
		{
			port = 2;
		}

		if(point > 15)
		{
			point = 15;
		}

		ret = rpnt471(&m_tCblk, port, point);
/*
		if(ret == 0)
		{
			ret = 1;
		}
		else
		{
			ret = 0;
		}
*/
	}
	else
	{
		ret = 0;
		cout << "CAp471::GetAt() FAILED : device not initialized" << endl;
	}

	return ret;
}

UINT16 CAp471::GetPort(UINT16 port)
{
	UINT16 ret = 0;

	if(m_tCblk.bInitialized)
	{
		if(port > 2)
		{
			port = 2;
		}

		ret = rprt471(&m_tCblk, port);
	}
	else
	{
		ret = 0;
		cout << "CAp471::GetPort() FAILED : device not initialized" << endl;
	}

	return ~ret;
}

BOOL CAp471::SetAt(UINT16 port, UINT16 point, UINT16 val)
{
	BOOL ret = FALSE;

	if(m_tCblk.bInitialized)
	{
		if(port > 2)
		{
			port = 2;
		}

		if(point > 15)
		{
			point = 15;
		}

		if(val == 0)
		{
			val = 1;
		}
		else
		{
			val = 0;
		}

		wpnt471(&m_tCblk, port, point, val); // api가 항상 0 리턴
		//wpnt471(&m_tCblk, port, point, val); // api가 항상 0 리턴
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
		cout << "CAp471::SetAt() FAILED : device not initialized" << endl;
	}

	return ret;
}

BOOL CAp471::SetPort(UINT16 port, UINT16 val)
{
	BOOL ret = FALSE;

	if(m_tCblk.bInitialized)
	{
		if(port > 2)
		{
			port = 2;
		}

		wprt471(&m_tCblk, port, ~val); // api가 항상 0을 리턴
		//wprt471(&m_tCblk, port, val); // api가 항상 0을 리턴
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
		cout << "CAp471::SetPort() FAILED : device not initialized" << endl;
	}

	return ret;
}

BOOL CAp471::SetMask(UINT16 port, UINT16 val)
{
	BOOL ret = FALSE;

	if(m_tCblk.bInitialized)
	{
		if(port > 2)
		{
			port = 2;
		}

		m_tCblk.param = 0xFF;
		m_tCblk.WriteMask[port] = static_cast<UINT32>(val & 0xFFFF);
		cnfg471(&m_tCblk);
		m_tCblk.param = 0x00;

		ret = TRUE;
	}
	else
	{
		ret = FALSE;
		cout << "CAp471::SetMask() FAILED : device not initialized" << endl;
	}

	return ret;
}

BOOL CAp471::SetMask(UINT16 port, UINT16 point, UINT16 val)
{
	BOOL ret = FALSE;

	if(m_tCblk.bInitialized)
	{
		UINT16 bpos;
		UINT32 temp;

		if(port > 2)
		{
			port = 2;
		}

		if(point > 15)
		{
			point = 15;
		}

		bpos = 1 << point;
		val <<= point;

		temp = m_tCblk.WriteMask[port];
		temp = (temp & ~bpos) | (val);

		m_tCblk.param = 0xFF;
		m_tCblk.WriteMask[port] = static_cast<UINT32>(temp & 0xFFFF);
		cnfg471(&m_tCblk);
		m_tCblk.param = 0x00;

		ret = TRUE;
	}
	else
	{
		ret = FALSE;
		cout << "CAp471::SetMask() FAILED : device not initialized" << endl;
	}
}

void CAp471::InterruptThread()
{
	cout << "START AP471_INTERRUPT_THREAD!!!" << endl;

	struct AP471_isr_info isr_info;
	int Status;

	while(m_tCblk.bAP)
	{
		memset(&isr_info, 0, sizeof(isr_info));

		Status = AP471Blocking_io(m_tCblk.nHandle, &isr_info, reinterpret_cast<long *>(&m_tCblk.brd_ptr->InterruptRegister), 1L, 2L);

		if(Status == 0 && isr_info.Status == 0)
		{
			break;
		}

		printf("\nISR EVPend[0]=0x%02X EVPend[1]=0x%02X EVPend[2]=0x%02X\n", isr_info.EventPending[0], isr_info.EventPending[1], isr_info.EventPending[2]);

		output_long(m_tCblk.nHandle, reinterpret_cast<long *>(&m_tCblk.brd_ptr->EventPendingClrReg[0]), static_cast<long>(isr_info.EventPending[0]));
		output_long(m_tCblk.nHandle, reinterpret_cast<long *>(&m_tCblk.brd_ptr->EventPendingClrReg[1]), static_cast<long>(isr_info.EventPending[1]));
		output_long(m_tCblk.nHandle, reinterpret_cast<long *>(&m_tCblk.brd_ptr->EventPendingClrReg[2]), static_cast<long>(isr_info.EventPending[2]));
	}

	cout << "STOP AP471_INTERRUPT_THREAD" << endl;
}

