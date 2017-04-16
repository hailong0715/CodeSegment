#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main()
{
	struct sockaddr_in sockaddr;
	int one =1;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(40714);
	sockaddr.sin_addr.s_addr  = INADDR_ANY;

	int clientfd;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0); //创建一个socket
	//将该套接字的绑定端口设为可重用
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*) &one,(socklen_t)sizeof(one));
	if(bind(listenfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		perror("bind error");
		return -1;
	}

	listen(listenfd, 16);
	
    struct sockaddr_in clientAdd;
    char buff[101] = {0};
    socklen_t len = sizeof(clientAdd);
    int closing =0;
	clientfd = accept(listenfd, (struct sockaddr *)&clientAdd, &len);
    int n;
    while((n = recv(clientfd,buff, 100,0 )) > 0)
    {
        printf("number of receive bytes = %d\n", n);
		//发送数据
        send(clientfd, buff, n, 0);
		char* bufTmp = buff;
        while(*bufTmp != '\r' && *bufTmp !='\n')
			bufTmp++;
		*bufTmp = '\0';
        if(strcmp(buff, "quit") == 0)
        {
            break;
        }
    }
	close(clientfd);
    close(listenfd);
	return 0;	
}
