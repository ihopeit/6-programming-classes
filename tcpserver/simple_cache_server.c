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
char *WRONG_ARGUMENTS = "ERR wrong number of arguments for command";
char *COMMAND_NOT_SUPPORTED = "Command not supported, supported commands: set key value, get key";

// Substr for characters in `src` between `m` and `n` (excluding `n`)
// Example: char* substring = substr(line, 5, strlen(line));
char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;
 
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    // start with m'th char and copy `len` chars into the destination
    strncpy(dest, (src + m), len);

    return dest;
}

int split (const char *txt, char delim, char ***tokens){
    int *tklen, *t, count = 1;
    char **arr, *p = (char *) txt;

    while (*p != '\0') if (*p++ == delim) count += 1;
    t = tklen = calloc (count, sizeof (int));
    for (p = (char *) txt; *p != '\0'; p++) *p == delim ? *t++ : (*t)++;
    *tokens = arr = malloc (count * sizeof (char *));
    t = tklen;
    p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
    while (*txt != '\0'){
        if (*txt == delim){
            p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
            txt++;
        }
        else *p++ = *txt++;
    }
    free (tklen);
    return count;
}

void free_tokens(char **tokens, int count){
	for (int i = 0; i < count; i++) free (tokens[i]);
        free (tokens);
}

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

		char **tokens;
        int count;
		buff[strlen(buff)-1] = 0; //remove the trailing newline
		if(strlen(buff) == 0 ){
			//fprintf(stderr, "no message from client:%s\n", buff);
			write(connfd, "\n", 1);

			int errnum = errno;
			if(errnum == EPIPE) {
				fprintf(stderr, "Pipe error, errno: %d\n", errno);
				errno = 0; //reset errno to 0;
				break;
			}
			continue;
		}
        count = split (buff, ' ', &tokens);
        
		if(count < 2) { // 消息不正确时，也回复客户端（避免客户端持续等待）
			write(connfd, WRONG_ARGUMENTS, strlen(WRONG_ARGUMENTS));
			free_tokens(tokens, count);
			continue;
		}

		if (strcmp(tokens[0], "set") == 0 && count >= 3)
		{
			int command_and_key_len = strlen(tokens[0]) + strlen(tokens[1]) + 2;
			char *value = substr(buff, command_and_key_len, strlen(buff));
			ht_set(cache, tokens[1], value);

			// and send respone to client
			write(connfd, "OK", strlen("OK"));
		}
		else if (strcmp(tokens[0], "get") == 0 && count == 2)
		{
			char *value = (char *)ht_get(cache, tokens[1]);
			if (value)
			{  // fprintf(stderr, "key:%s, value:%s", tokens[1], value);
				write(connfd, value, strlen(value));
			}
			else
			{  // fprintf(stderr, "Error getting key:%s, value:%s", tokens[1], value);
				write(connfd, "\n", 1);
			}
		}
		else
		{
			write(connfd, COMMAND_NOT_SUPPORTED, strlen(COMMAND_NOT_SUPPORTED));
		}
		free_tokens(tokens, count);

		int errnum = errno;
		if(errnum == EPIPE) {
			fprintf(stderr, "Pipe error, errno: %d\n", errno);
			errno = 0; //reset errno to 0;
			break;
		}
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

	// 开始监听，等待客户端连接，设置最大等待队列长度
	if ((listen(sockfd, 2)) != 0) {
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

