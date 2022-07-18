#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include <string>

using namespace std;

class CFile
{
public:
	CFile();
	~CFile();

protected:
	FILE * m_pStream;
	string m_strName;

public:
	bool Open(const char * path, const char * mode);
	void Close();
	const char * Name();
	size_t Read(char * ptr, size_t size);
	size_t Write(const char * ptr, size_t size);

public:
	static int Size(const char * path);

public:
	operator FILE * () const;

public:
	static int Rename(const char *old, const char * ch);
};


class CTextFile : public CFile
{
public:
	CTextFile();
	~CTextFile();

public:
	size_t GetString(char * ptr, size_t size);
	size_t PutString(const char * ptr, size_t size);

public:
	operator FILE * () const;
};

