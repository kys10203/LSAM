#pragma once

class CConfig
{
public:
	CConfig();
	~CConfig();

public:
	static bool GetConfig(const char * path, const char * cfgname, char * cfgval, unsigned int cfglen);
	static bool SetConfig(const char * path, const char * cfgname, const char * cfgval);
};

