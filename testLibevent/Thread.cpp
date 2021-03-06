#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>



void* doRead(void* arg)
{
	long connfd = (long)arg;
	char recvBuf[101] = "";
	int n = 0; 
    while((n = recv(connfd,recvBuf, sizeof(recvBuf),0 )) > 0)
    {
        printf("number of receive bytes = %d.\n", n);
		//发送数据
        send(connfd, recvBuf, n, 0);
		char* bufTmp = recvBuf;
        while(*bufTmp != '\r' && *bufTmp !='\n')
			bufTmp++;
		*bufTmp = '\0';
        if(strcmp(recvBuf, "quit") == 0)
        {
            break;
        }
    }
	close(connfd);
 
	return NULL;
}
int main()
{
	struct sockaddr_in sockaddr;
	pthread_t thread_id;
	int one =1;
	int ret = 0;
	bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8080);
	sockaddr.sin_addr.s_addr  = htonl(INADDR_ANY);
	long clientfd;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0); //创建一个socket
	//将该套接字的绑定端口设为可重用
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*) &one,(socklen_t)sizeof(one));
	if(bind(listenfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		perror("bind error");
		return -1;
	}

	ret = listen(listenfd, 16);
	if(ret < 0)
	{
		perror("listen failed.\n");
		return -1;
	}
	
    struct sockaddr_in clientAdd;
    socklen_t len = sizeof(clientAdd);
	while(1)
	{
		clientfd = accept(listenfd, (struct sockaddr *)&clientAdd, &len);
		if(clientfd < 0)
		{
			perror("accept this time");
			continue;
		}
		if(clientfd > 0)
		{
			//由于同一个进程内的所有线程共享内存和变量，因此在传递参数时需作特殊处理，值传递
			pthread_create(&thread_id, NULL, doRead, (void *)clientfd);
			printf("thread %d created.\n",thread_id ); 
			pthread_detach(thread_id);
		}
	}
	close(listenfd);
	return 0;	
}
     

