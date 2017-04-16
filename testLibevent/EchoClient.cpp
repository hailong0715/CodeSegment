#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* DoSend(void* arg)
{
	struct sockaddr_in serAddr;
	int connfd = socket(AF_INET, SOCK_STREAM, 0);
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8080);
	serAddr.sin_addr.s_addr = INADDR_ANY;
	int ret = connect(connfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
	while(1)
	{
		char sendbuf[101];
		char recvBuf[101] = {0};
		sprintf(sendbuf, "msg from thread %ul\n", pthread_self());
		send(connfd,sendbuf, strlen(sendbuf)+1, 0);
		int n = recv(connfd, recvBuf, sizeof(recvBuf),0);
		if(n>0)
			printf("msg from server is %s\n",recvBuf);
	}
}

int main()
{
	for(int i=0; i<2000; i++)
	{
		pthread_t tid;
		pthread_create(&tid, NULL, DoSend, NULL);
		
	}
	while(1);
	return 0;
}
