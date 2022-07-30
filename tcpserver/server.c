#include <stdio.h>
#include <unistd.h> // for read(), write()
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#include "../structure/hashtable.h"

#define MAX 256
#define PORT 8080

extern int errno ;

// function to read from client, and send response
void func(int connfd, ht* cache)
{
	errno = 0; //to set errno to 0 at initializing
	char buff[MAX];
	char response[MAX];
	int n;
	// infinite loop for reading message from client
	for (;;) {
		memset(buff, 0, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));

		// length of the input string:
		memset(response, 0, MAX);
		snprintf(response, sizeof response, "length of input:%lu", strlen(buff));

		// and send the result to client
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


int main()
{
	ht* cache = ht_create();

	int sockfd, connfd; 
	unsigned int len;
	struct sockaddr_in servaddr, cli;

    // Set the SIGPIPE handler to SIG_IGN. 
	// This will prevent socket write from causing a SIGPIPE signal.
    signal(SIGPIPE, SIG_IGN);

	// socket create and verification
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = PF_INET;
	// 主机字节顺序到网络字节顺序的转换
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);// 0.0.0.0 地址对应的数字
	servaddr.sin_port = htons(PORT); //host to network short

	// Binding newly created socket to given IP and verification
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
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

    for(;;){ // 持续监听来自客户端的连接请求
		// Accept the data packet from client and verification
		connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
		if (connfd < 0) {
			printf("server accept failed...\n");
			exit(0);
		}
		else
			printf("server accept the client...\n");

		// Function for communication between client and server
		func(connfd, cache);
	}

	// After communication close the socket
	close(sockfd);

	ht_destroy(cache);
}

