/*
 * a TCP Server which reads text from client, and response with the text length
 * nc localhost 8080
 * @file server.c
 * @author david
 * @brief 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <unistd.h> // for read(), write()
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define MAX 256
#define PORT 8080

extern int errno ;

// 处理与客户端的通信，读取输入字符串数据，计算字符串长度，并响应客户端
void func(int connfd){ 
	errno = 0; //to set errno to 0 at initializing
	char buff[MAX], response[MAX];
	int n;
	// infinite loop for reading message from client
	for (;;) {
		memset(buff, 0, MAX);
		memset(response, 0, MAX);

		// 从客户端 socket 读取内容，保存到 buff 缓冲区
		read(connfd, buff, sizeof(buff));

		// 计算读取到的文本长度，并且回复客户端计算出来的文本长度
		snprintf(response, sizeof response, "input len:%lu\n", strlen(buff));
		write(connfd, response, sizeof(response));

		int errnum = errno;
		if(errnum == EPIPE) {
			fprintf(stderr, "Pipe Error, errno: %d\n", errno);
			errno = 0; //reset errno to 0;
			break;
		}

		// print buffer which contains the client contents
		printf("From client: %s  To client: %d %s\n", buff, connfd, response);
	}
}


int main(){
	int sockfd, connfd; unsigned int len;
	struct sockaddr_in servaddr, cli;
    signal(SIGPIPE, SIG_IGN); // 忽略 SIGPIPE 信号避免 crash

	sockfd = socket(PF_INET, SOCK_STREAM, 0); // create socket
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	} else 
		printf("Socket successfully created..\n"); 

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET; // protocol family: internet
	// 主机字节顺序到网络字节顺序的转换, INADDR_ANY 是 0.0.0.0 地址对应的数字
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT); //host to network short

	// 绑定 socket 到 IP 地址， 0.0.0.0 表示本机所有的地址
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	} else
		printf("Server listening..\n");
	len = sizeof(cli);

    for(;;){ // 持续监听来自客户端的连接请求，接收客户端请求：
		connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
		if (connfd < 0) {
			printf("server accept failed...\n");
			exit(0);
		} else
			printf("server accept the client...\n");

		func(connfd); // 通过连接socket connfd 处理客户端服务端的通信
	}

	close(sockfd); 	// After communication close the socket
}
