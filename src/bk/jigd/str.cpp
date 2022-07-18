#include "str.h"

CString::CString()
{
}

CString::~CString()
{
}

string & CString::Replace(string & str, const char * old, const char * ch)
{
	string oldstr(old), chstr(ch);

	// old가 ch에 포함되면 무한루프 돎
	if(chstr.find(oldstr) == string::npos)
	{
		while(str.find(oldstr) != string::npos)
		{
			str.replace(str.find(oldstr), oldstr.length(), chstr);
		}
	}
	else
	{
		// 무한루프 돌지 않도록 한 번만 변경
		str.replace(str.find(oldstr), oldstr.length(), chstr);
	}

	return str;
}

string & CString::Trim(string & str)
{
	string sp(" "), tab("\t");

	while(str.find(sp) == 0)
	{
		str.erase(0, 1);
	}

	while(str.find(tab) == 0)
	{
		str.erase(0, 1);
	}

	return str;
}

