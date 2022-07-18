#include "C16bit.h"
#include <string.h>

C8bit::C8bit(void)
{
	b0 = 0;
	b1 = 0;
	b2 = 0;
	b3 = 0;
	b4 = 0;
	b5 = 0;
	b6 = 0;
	b7 = 0;
}

C8bit::C8bit(const C8bit & cp)
{
	b0 = cp.b0;
	b1 = cp.b1;
	b2 = cp.b2;
	b3 = cp.b3;
	b4 = cp.b4;
	b5 = cp.b5;
	b6 = cp.b6;
	b7 = cp.b7;
}

C8bit::~C8bit(void)
{
}

C8bit::operator unsigned char() const
{
	unsigned char ret;

	memcpy(&ret, this, sizeof(ret));

	return ret;
};

C8bit & C8bit::operator = (const unsigned char & val)
{
	memcpy(this, &val, sizeof(val));

	return * this;
};

C8bit & C8bit::operator = (const C8bit & cp)
{
	b0 = cp.b0;
	b1 = cp.b1;
	b2 = cp.b2;
	b3 = cp.b3;
	b4 = cp.b4;
	b5 = cp.b5;
	b6 = cp.b6;
	b7 = cp.b7;

	return * this;
}


C16bit::C16bit(void)
{
	b0 = 0;
	b1 = 0;
	b2 = 0;
	b3 = 0;
	b4 = 0;
	b5 = 0;
	b6 = 0;
	b7 = 0;
	b8 = 0;
	b9 = 0;
	b10 = 0;
	b11 = 0;
	b12 = 0;
	b13 = 0;
	b14 = 0;
	b15 = 0;
}


C16bit::C16bit(const C16bit & cp)
{
	b0 = cp.b0;
	b1 = cp.b1;
	b2 = cp.b2;
	b3 = cp.b3;
	b4 = cp.b4;
	b5 = cp.b5;
	b6 = cp.b6;
	b7 = cp.b7;
	b8 = cp.b8;
	b9 = cp.b9;
	b10 = cp.b10;
	b11 = cp.b11;
	b12 = cp.b12;
	b13 = cp.b13;
	b14 = cp.b14;
	b15 = cp.b15;
}

C16bit::~C16bit(void)
{
}

C16bit::operator unsigned short() const
{
	unsigned short ret;

	memcpy(&ret, this, sizeof(ret));

	return ret;
};

C16bit & C16bit::operator = (const unsigned short & val)
{
	memcpy(this, &val, sizeof(val));

	return * this;
};

C16bit & C16bit::operator = (const C16bit & cp)
{
	b0 = cp.b0;
	b1 = cp.b1;
	b2 = cp.b2;
	b3 = cp.b3;
	b4 = cp.b4;
	b5 = cp.b5;
	b6 = cp.b6;
	b7 = cp.b7;
	b8 = cp.b8;
	b9 = cp.b9;
	b10 = cp.b10;
	b11 = cp.b11;
	b12 = cp.b12;
	b13 = cp.b13;
	b14 = cp.b14;
	b15 = cp.b15;

	return * this;
}
