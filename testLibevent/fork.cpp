#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void doread(int fd)
{
	int n = 0;
	char buf[101] = {0};
	while((n = recv(fd, buf, sizeof(buf), 0))> 0)
	{
		printf("msg recv is %s\n", buf);
		send(fd,buf, sizeof(buf), 0);
		char* bufTmp = buf;
        while(*bufTmp != '\r' && *bufTmp !='\n')
			bufTmp++;
		*bufTmp = '\0';
        if(strcmp(buf, "quit") == 0)
        {
            break;
        }		
	}
}

int main()
{
	struct sockaddr_in SerAddr;
	SerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SerAddr.sin_port = htons(12345);
	SerAddr.sin_family = AF_INET;

	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(bind(listenfd, (struct sockaddr*)&SerAddr, sizeof(SerAddr)) < 0)
	{
		perror("bind failed");
		return -1;
	}

	if(listen(listenfd, 64) < 0)
	{
		perror("listen failed\n");
		return -1;
	}

	while(1)
	{
		int connfd;
		struct sockaddr_in clientAddr;
		socklen_t len = sizeof(clientAddr);
		connfd = accept(listenfd, (struct sockaddr*)&clientAddr, &len);
		if(connfd < 0)
			continue;
		if(fork() == 0)
		{
			close(listenfd);
			doread(connfd);
			close(connfd);
		}

		close(connfd);

	}
	
}
