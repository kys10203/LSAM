#pragma once

#include "dev.h"
#include <termios.h>

class CTty : public CDev
{
public:
	CTty();
	~CTty();

protected:
	termios m_tTermIos;
	termios m_tOldTermIos;

public:
	void InitTerm(int baudrate = 9600, int rtscts = 0, int cs = 8, int canon = 0);
	bool Open(const char * path, int flag = O_RDWR | O_NOCTTY);
	void Close();
};

