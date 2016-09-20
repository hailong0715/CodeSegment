/*CConfigFileReader 类用于读取配置文件中的各个配置项，配置项形式KEY = VALUE */
#include <iostream>
#include <map>
using namespace std;
class CConfigFileReader
{
public:
	CConfigFileReader(const char* filename);
	~CConfigFileReader();
	char* GetConfigValue(const char* name);
	int SetConfigValue(const char * name, const char* value);

private:

	void _LoadFile(const char* filename);
	int _WriteFile(const char* filename = nullptr);
	void _ParseLine(char* line);
	char* _TrimSpace(char* name);

	bool m_load_ok;
	map<string, string> m_config_map;
	string m_config_file;
};