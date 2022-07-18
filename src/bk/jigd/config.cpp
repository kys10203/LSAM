#include "config.h"
#include "file.h"
#include "str.h"

#include <iostream>

using namespace std;

#define MAX_CONFIG_LENGTH	(512)

CConfig::CConfig()
{
}

CConfig::~CConfig()
{
}

bool CConfig::GetConfig(const char * path, const char * cfgname, char * cfgval, unsigned int cfglen)
{
	bool ret = false;
	CTextFile src;
	char buf[MAX_CONFIG_LENGTH] = {0, };
	string val = "";

	if(src.Open(path, "r") == true)
	{
		while(feof(src) == 0)
		{
			if(src.GetString(buf, sizeof(buf)) > 0)
			{
				string line(buf);

				line = CString::Trim(line);

				if(line[0] != '#')
				{
					if(line.find(cfgname) != string::npos)
					{
						val = line.substr(line.find("=") + 1, string::npos);
						val = CString::Replace(val, " ", "");
						val = CString::Replace(val, "\t", "");
						break;
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else
			{
			}
		}

		if(val.length() == 0)
		{
			ret = false;
		}
		else
		{
			if(cfglen >= val.length())
			{
				strncpy(cfgval, val.c_str(), val.length());
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

bool CConfig::SetConfig(const char * path, const char * cfgname, const char * cfgval)
{
	bool ret = false;
	CTextFile src, bk;
	char buf[MAX_CONFIG_LENGTH] = {0, };
	string tmp(path);

	if(src.Open(path, "r") == true)
	{
		tmp = CString::Replace(tmp, ".cfg", ".bakcfg");

		if(bk.Open(tmp.c_str(), "w+") == true)
		{
			while(feof(src) == 0)
			{
				if(src.GetString(buf, sizeof(buf)) > 0)
				{
					string line(buf);

					line = CString::Trim(line);

					if(line.find(cfgname) != string::npos && line[0] != '#')
					{
						sprintf(buf, "%s = %s\n", cfgname, cfgval);
						bk.PutString(buf, strlen(buf));

						ret = true;
					}
					else
					{
						bk.PutString(buf, strlen(buf));
					}
				}
				else
				{
				}
			}

			if(ret == false)
			{
				sprintf(buf, "%s = %s\n", cfgname, cfgval);
				bk.PutString(buf, strlen(buf));

				ret = true;
			}
			else
			{
			}

			bk.Close();
		}
		else
		{
			ret = false;
		}

		src.Close();
	}
	else
	{
		ret = false;
	}

	if(ret == true)
	{
		CFile::Rename(tmp.c_str(), path);
	}
	else
	{
	}

	return ret;
}

