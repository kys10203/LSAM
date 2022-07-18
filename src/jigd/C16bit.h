#pragma once

// 비트필드 클래스

class C8bit
{
public:
	C8bit(void);
	C8bit(const C8bit & cp);
	~C8bit(void);

public:
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;

	//unsigned char b7 : 1;
	//unsigned char b6 : 1;
	//unsigned char b5 : 1;
	//unsigned char b4 : 1;
	//unsigned char b3 : 1;
	//unsigned char b2 : 1;
	//unsigned char b1 : 1;
	//unsigned char b0 : 1;

public:
	operator unsigned char() const;
	C8bit & operator = (const unsigned char & val);
	C8bit & operator = (const C8bit & cp);
};

class C16bit
{
public:
	C16bit(void);
	C16bit(const C16bit & cp);
	~C16bit(void);

public:
	unsigned short b0 : 1;
	unsigned short b1 : 1;
	unsigned short b2 : 1;
	unsigned short b3 : 1;
	unsigned short b4 : 1;
	unsigned short b5 : 1;
	unsigned short b6 : 1;
	unsigned short b7 : 1;
	unsigned short b8 : 1;
	unsigned short b9 : 1;
	unsigned short b10 : 1;
	unsigned short b11 : 1;
	unsigned short b12 : 1;
	unsigned short b13 : 1;
	unsigned short b14 : 1;
	unsigned short b15 : 1;

	//C8bit h;
	//C8bit l;

public:
	operator unsigned short() const;
	C16bit & operator = (const unsigned short & val);
	C16bit & operator = (const C16bit & cp);
};
