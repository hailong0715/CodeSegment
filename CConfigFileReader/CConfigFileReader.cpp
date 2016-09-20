#include "CConfigFileReader.h"
#include <stdio.h>
CConfigFileReader::CConfigFileReader(const char* filename)
{
	_LoadFile(filename);
}

CConfigFileReader::~CConfigFileReader()
{}

char* CConfigFileReader::GetConfigValue(const char* name)
{
	if (!m_load_ok)
		return NULL;
	char* value = NULL;
	map<string, string>::iterator it = m_config_map.find(name);
	if (it != m_config_map.end())
	{
		value = (char*)it->second.c_str();
	}
	return value;
}

int CConfigFileReader::SetConfigValue(const char* name, const char* value)
{
	if (!m_load_ok)
		return -1;

	map<string, string>::iterator it = m_config_map.find(name);
	if (it != m_config_map.end())
	{
		it->second = value;
	}
	else
	{
		m_config_map.insert(make_pair(name, value));
	}
	return _WriteFile();;
}

void CConfigFileReader::_LoadFile(const char* filename)
{
	m_config_file.clear();
	m_config_file.append(filename);
	FILE *fp ;
	fopen_s(&fp, filename, "r");
	if (!fp)
	{
		printf("Can not open config file %s error = %d", filename, errno);
		return;
	}

	char buf[256];
	for (;;)
	{
		char* p = fgets(buf, 256, fp);
		if (!p)
			break;
		size_t len = strlen(p);
		if (buf[len - 1] == '\n')  //删除行末尾的换行符
			buf[len - 1] = 0;

		char* ch = strchr(buf, '#'); //删除以#号开始的注释
		if (ch)
			*ch = 0;  //若这行是注释，则将该行的第一个字符设置为'\0'
		if (strlen(buf) == 0)
			continue;
		_ParseLine(buf);
	}
	fclose(fp);
	m_load_ok = true;
}

int CConfigFileReader::_WriteFile(const char * filename)
{
	FILE* fp = NULL;
	if (NULL == filename)
	{
		fopen_s(&fp,m_config_file.c_str(), "w");
	}
	else
	{
		fopen_s(&fp,filename, "w");
	}

	if (!fp)
		return -1;

	char szConfigPair[128] = { 0 };
	map<string, string>::iterator it = m_config_map.begin();
	for (; it != m_config_map.end(); it++)
	{
		sprintf_s(szConfigPair, "%s=%s\n", it->first.c_str(), it->second.c_str());
		unsigned int ret = fwrite(szConfigPair, sizeof(szConfigPair), 1, fp);
		if (1 != ret)
		{
			fclose(fp);
			return -1;
		}
	}
	fclose(fp);
	return 0;
}

void CConfigFileReader::_ParseLine(char* line)
{
	if (NULL == line)
		return;
	char* ch = strchr(line, '=');
	if (!ch)
		return;
	*ch = 0;
	char* key = _TrimSpace(line);
	char* value = _TrimSpace(ch + 1);

	if (key && value)
	{
		m_config_map.insert(make_pair(key, value));
	}

}

char* CConfigFileReader::_TrimSpace(char* name)
{
	if (NULL == name)
		return NULL;
	char* start_pos = name;
	while (*start_pos == ' ' || *start_pos == '\t')
		start_pos++;
	if (strlen(start_pos) == 0)
		return NULL;
	char* end_pos = name + strlen(name) - 1;
	while (*end_pos == ' ' || *end_pos == '\t')
	{
		*end_pos = 0;
		end_pos--;
	}

	int len = end_pos - start_pos + 1;
	if (len <= 0)
		return NULL;
	return start_pos;
	
}