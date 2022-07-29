#include <stdio.h>
#include <unistd.h> // for read(), write()
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 256
#define PORT 8080

// function to read from client, and send response
void func(int connfd)
{
	char buff[MAX];
	char response[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		memset(buff, 0, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));

		// length of the input string:
		memset(response, 0, MAX);
		snprintf(response, sizeof response, "length of input:%lu", strlen(buff));

		// and send the result to client
		write(connfd, response, sizeof(response));

		// print buffer which contains the client contents
		printf("From client: %s  To client: %s\n", buff, response);
	}
}


int main()
{
	int sockfd, connfd; 
	unsigned int len;
	struct sockaddr_in servaddr, cli;

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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

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

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for communication between client and server
	func(connfd);

	// After communication close the socket
	close(sockfd);
}

