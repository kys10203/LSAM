#pragma once

#include <string>
#include "apptype.h"

using namespace std;

class CDate
{
public:
	CDate();
	CDate(INT32 year, INT32 mon, INT32 day);
	~CDate();

public:
	UINT32 YYYY;
	UINT32 MM;
	UINT32 DD;
	UINT32 hh;
	UINT32 mm;
	UINT32 ss;

public:
	void SetDate(INT32 year, INT32 mon, INT32 day, INT32 hour = 0, INT32 minute = 0, INT32 second = 0);

public:
	INT32 SetSystemDate(UINT32 nYear, UINT32 nMonth, UINT32 nDay, UINT32 nHour, UINT32 nMinute, UINT32 nSecond);
	string GetSystemDate();

public:
	static INT32 ThisYear();
	static INT32 ThisMonth();
	static INT32 Today();
	static INT32 Hour();
	static INT32 Min();
	static INT32 Sec();
	static INT32 DayOfTheWeek();
	static INT32 DayOfTheWeek(INT32 year, INT32 mon, INT32 day);
	static INT32 EndDay(INT32 year, INT32 mon);
	static bool IsLeapYear(INT32 year);
	static CDate NextDay(INT32 year, INT32 mon, INT32 day);
	static INT32 SpanOfDays(CDate start, CDate end);
	static FLOAT64 SpanTime(CDate start, CDate end);

public:
	CDate & operator = (CDate & date);
	CDate & operator + (INT32 & span);
	CDate & operator ++ (INT32);
	CDate & operator - (INT32 & span);
	CDate & operator -- (INT32);
	bool operator == (CDate & date);
};

