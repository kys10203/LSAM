#pragma once

#include <string>

using namespace std;

class CString
{
public:
	CString();
	~CString();

public:
	static string & Replace(string & str, const char * old, const char * ch);
	static string & Trim(string & str);
};

