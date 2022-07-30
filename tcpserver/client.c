#include <netdb.h>
#include <stdio.h>
#include <unistd.h> // for read(), write()
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_addr

#define MAX 256
#define PORT 8080

void func(int sockfd, char *server_addr){
	char buff[MAX], response[MAX];
	int n;
	for (;;) {
		memset(buff, 0, sizeof(buff));
		printf("%s>",server_addr);
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
		write(sockfd, buff, sizeof(buff));
		memset(response, 0, sizeof(response));
		read(sockfd, response, sizeof(response));
		printf("%s\n", response);
	}
}

int main(int argc, char** argv){
	char* server_addr = argc > 1 ? argv[1] : "127.0.0.1";
	struct sockaddr_in servaddr;

	int sockfd = socket(PF_INET, SOCK_STREAM, 0); // create socket
	if (sockfd == -1) {
        printf("socket creation failed...\n");
	    exit(0);
	} else { printf("Socket successfully created..\n"); }

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server_addr);
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	} else { printf("connected to the server..\n"); }

	func(sockfd, server_addr); // 处理客户端服务端连接
	close(sockfd); // close the socket
}

