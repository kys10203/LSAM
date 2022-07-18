#include "file.h"

CFile::CFile()
{
	m_pStream = NULL;
}

CFile::~CFile()
{
	Close();
}

bool CFile::Open(const char * path, const char * mode)
{
	bool ret = false;

	if(m_pStream != NULL)
	{
		Close();
	}
	else
	{
	}

	m_pStream = fopen(path, mode);

	if(m_pStream == NULL)
	{
		ret = false;
	}
	else
	{
		m_strName = path;
		ret = true;
	}

	return ret;
}

void CFile::Close()
{
	if(m_pStream != NULL)
	{
		fclose(m_pStream);
		m_pStream = NULL;
	}
	else
	{
	}

	m_strName = "";
}

const char * CFile::Name()
{
	return m_strName.c_str();
}

size_t CFile::Read(char * ptr, size_t size)
{
	size_t ret = 0;

	if(m_pStream == 0)
	{
		ret = 0;
	}
	else
	{
		ret = fread(ptr, sizeof(char), size, m_pStream);
	}

	return ret;
}

size_t CFile::Write(const char * ptr, size_t size)
{
	size_t ret = 0;

	if(m_pStream == 0)
	{
		ret = 0;
	}
	else
	{
		ret = fwrite(ptr, sizeof(char), size, m_pStream);
	}

	return ret;
}

int CFile::Size(const char * path)
{
	int ret = -1;
	struct stat st = {0};

	if(stat(path, &st) == -1)
	{
		ret = -1;
	}
	else
	{
		ret = static_cast<int>(st.st_size);
	}

	return ret;
}

CFile::operator FILE * () const
{
	return m_pStream;
}

int CFile::Rename(const char * old, const char * ch)
{
	return rename(old, ch);
}


CTextFile::CTextFile()
{
}

CTextFile::~CTextFile()
{
}

size_t CTextFile::GetString(char * ptr, size_t size)
{
	size_t ret = 0;

	if(m_pStream == 0)
	{
		ret = 0;
	}
	else
	{
		if(fgets(ptr, size, m_pStream) == NULL)
		{
			ret = 0;
		}
		else
		{
			ret = strlen(ptr);
		}
	}

	return ret;
}

size_t CTextFile::PutString(const char * ptr, size_t size)
{
	size_t ret = 0;

	if(m_pStream == 0)
	{
		ret = 0;
	}
	else
	{
		ret = fputs(ptr, m_pStream);
		//ret += putc('\n', m_pStream);
	}

	return ret;
}

CTextFile::operator FILE * () const
{
	return m_pStream;
}

