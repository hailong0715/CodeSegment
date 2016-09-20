#include"CConfigFileReader.h"

int main()
{
	CConfigFileReader confList("config.conf");
	char* listenip = confList.GetConfigValue("listenip");
	char* port = confList.GetConfigValue("port");
	char* peer_address = confList.GetConfigValue("peer_address");
	char* peer_port = confList.GetConfigValue("peer_port");

	cout << listenip << endl << port << endl << peer_address << endl << peer_port;
	return 0;

}