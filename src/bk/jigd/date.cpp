#include "date.h"
#include <iostream>

using namespace std;

CDate::CDate()
{
	YYYY = 0;
	MM = 0;
	DD = 0;
	hh = 0;
	mm = 0;
	ss = 0;
}

CDate::CDate(INT32 year, INT32 mon, INT32 day)
{
	SetDate(year, mon, day);
}

CDate::~CDate()
{
}

void CDate::SetDate(INT32 year, INT32 mon, INT32 day, INT32 hour, INT32 minute, INT32 second)
{
	YYYY = year;
	MM = mon;
	DD = day;
	hh = hour;
	mm = minute;
	ss = second;
}

INT32 CDate::SetSystemDate(UINT32 nYear, UINT32 nMonth, UINT32 nDay, UINT32 nHour, UINT32 nMinute, UINT32 nSecond)
{
	INT32 nRet = -1;

	if(nYear < 2020)
	{
		nYear = 2020;
	}
	else
	{
	}

	if(nMonth < 1 || nMonth > 12)
	{
		nMonth = 1;
	}
	else
	{
	}

	if(nDay < 1 || nDay > 31)
	{
		nDay = 1;
	}
	else
	{
	}

	if(nHour > 23)
	{
		nHour = 0;
	}
	else
	{
	}

	if(nMinute > 59)
	{
		nMinute = 0;
	}
	else
	{
	}

	if(nSecond > 59)
	{
		nSecond = 0;
	}
	else
	{
	}

	YYYY = nYear;
	MM = nMonth;
	DD = nDay;
	hh = nHour;
	mm = nMinute;
	ss = nSecond;

	string cmd;

	cmd = "date -s \"" + to_string(YYYY) + "-" + to_string(MM) + "-" + to_string(DD);
	cmd = cmd + " " + to_string(hh) + ":" + to_string(mm) + ":" + to_string(ss) + "\"";

	cout << "DATE CMD = " << cmd << endl;

	cmd = "hwclock --localtime --systohc";

	cout << "HWCLOCK CMD = " << cmd << endl;

	nRet = 0;

	return nRet;
}

string CDate::GetSystemDate()
{
	string strRet = "";

	return strRet;
}

INT32 CDate::EndDay(INT32 year, INT32 mon)
{
	INT32 eday = -1;

	if(mon == 1)
	{
		eday = 31;
	}
	else if(mon == 2)
	{
		if(IsLeapYear(year) == true)
		{
			eday = 29;
		}
		else
		{
			eday = 28;
		}
	}
	else if(mon == 3)
	{
		eday = 31;
	}
	else if(mon == 4)
	{
		eday = 30;
	}
	else if(mon == 5)
	{
		eday = 31;
	}
	else if(mon == 6)
	{
		eday = 30;
	}
	else if(mon == 7)
	{
		eday = 31;
	}
	else if(mon == 8)
	{
		eday = 31;
	}
	else if(mon == 9)
	{
		eday = 30;
	}
	else if(mon == 10)
	{
		eday = 31;
	}
	else if(mon == 11)
	{
		eday = 30;
	}
	else if(mon == 12)
	{
		eday = 31;
	}
	else
	{
	}

	return eday;
}

bool CDate::IsLeapYear(INT32 year)
{
	bool ret = false;

	if(year % 4 == 0)
	{
		if(year % 100 != 0)
		{
			ret = true;
		}
		else
		{
			if(year % 400 == 0)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
		}
	}
	else
	{
		ret = false;
	}

	return ret;
}

INT32 CDate::Hour()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_hour;
}

INT32 CDate::Min()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_min;
}

INT32 CDate::Sec()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_sec;
}

INT32 CDate::ThisYear()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_year + 1900;
}

INT32 CDate::ThisMonth()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_mon + 1;
}

INT32 CDate::Today()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_mday;
}

INT32 CDate::DayOfTheWeek()
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	return dt->tm_wday;
}

INT32 CDate::DayOfTheWeek(INT32 year, INT32 mon, INT32 day)
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = year - 1900;
	dt->tm_mon = mon - 1;
	dt->tm_mday = day;
	mktime(dt);

	return dt->tm_wday;
}

CDate CDate::NextDay(INT32 year, INT32 mon, INT32 day)
{
	CDate ret;
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = year - 1900;
	dt->tm_mon = mon - 1;
	dt->tm_mday = day + 1;
	mktime(dt);


	ret.YYYY = dt->tm_year + 1900;
	ret.MM = dt->tm_mon + 1;
	ret.DD = dt->tm_mday;

	return ret;
}

INT32 CDate::SpanOfDays(CDate start, CDate end)
{
	CDate ret;
	time_t t = time(0);
	time_t t2 = time(0);
	tm * dt = localtime(&t);
	tm * dt2 = localtime(&t2);

	dt->tm_year = start.YYYY - 1900;
	dt->tm_mon = start.MM - 1;
	dt->tm_mday = start.DD;
	t = mktime(dt);


	dt2->tm_year = end.YYYY - 1900;
	dt2->tm_mon = end.MM - 1;
	dt2->tm_mday = end.DD;
	t2 = mktime(dt2);


	t2 = t2- t;

	return t2 / (24 * 60 * 60);
}

FLOAT64 CDate::SpanTime(CDate start, CDate end)
{
	CDate ret;
	time_t t = time(0);
	time_t t2 = time(0);
	tm * dt = localtime(&t);
	tm * dt2 = localtime(&t2);

	dt->tm_year = start.YYYY - 1900;
	dt->tm_mon = start.MM - 1;
	dt->tm_mday = start.DD;
	t = mktime(dt);


	dt2->tm_year = end.YYYY - 1900;
	dt2->tm_mon = end.MM - 1;
	dt2->tm_mday = end.DD;
	t2 = mktime(dt2);


	//t2 = t2- t;
	return difftime(t2, t);
}

CDate & CDate::operator = (CDate & date)
{
	YYYY = date.YYYY;
	MM = date.MM;
	DD = date.DD;
	hh = date.hh;
	mm = date.mm;
	ss = date.ss;

	return *this;
}

bool CDate::operator == (CDate & date)
{
	bool ret = false;

	if(YYYY == date.YYYY && MM == date.MM && DD == date.DD)
	{
		if(hh == date.hh && mm == date.mm && date.ss == date.ss)
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	return ret;
}

CDate & CDate::operator + (INT32 & span)
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = YYYY - 1900;
	dt->tm_mon = MM - 1;
	dt->tm_mday = DD + span;
	t = mktime(dt);

	YYYY = dt->tm_year + 1900;;
	MM = dt->tm_mon + 1;
	DD = dt->tm_mday;
	hh = dt->tm_hour;
	mm = dt->tm_min;
	ss = dt->tm_sec;

	return *this;
}

CDate & CDate::operator ++ (INT32)
		{
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = YYYY - 1900;
	dt->tm_mon = MM - 1;
	dt->tm_mday = DD + 1;
	t = mktime(dt);

	YYYY = dt->tm_year + 1900;;
	MM = dt->tm_mon + 1;
	DD = dt->tm_mday;
	hh = dt->tm_hour;
	mm = dt->tm_min;
	ss = dt->tm_sec;

	return *this;
		}

CDate & CDate::operator - (INT32 & span)
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = YYYY - 1900;
	dt->tm_mon = MM - 1;
	dt->tm_mday = DD - span;
	t = mktime(dt);

	YYYY = dt->tm_year + 1900;;
	MM = dt->tm_mon + 1;
	DD = dt->tm_mday;
	hh = dt->tm_hour;
	mm = dt->tm_min;
	ss = dt->tm_sec;

	return *this;
}

CDate & CDate::operator -- (INT32)
{
	time_t t = time(0);
	tm * dt = localtime(&t);

	dt->tm_year = YYYY - 1900;
	dt->tm_mon = MM - 1;
	dt->tm_mday = DD - 1;
	t = mktime(dt);

	YYYY = dt->tm_year + 1900;;
	MM = dt->tm_mon + 1;
	DD = dt->tm_mday;
	hh = dt->tm_hour;
	mm = dt->tm_min;
	ss = dt->tm_sec;

	return *this;
}

