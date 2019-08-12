#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define MAX_MSG_LEN 1000

int main(int argc, char *argv[]){
	int sock, connect_return_val;

	struct sockaddr_in client_addr, server_addr;

	char buffer[MAX_MSG_LEN];

	if(argc < 5){
		printf("usage: %s <Server IP> <Server Port> <Filename_Client> <Filename_Server>", argv[0]);
		exit(1);
	}

	bzero((char *) & server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Cannot Open Socket");
		exit(1);
	}

	connect_return_val = connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(connect_return_val < 0){
		perror("Cannot Connect");
		exit(1);
	}

	FILE *fp = fopen(argv[3], "r");

	send(sock, argv[4], strlen(argv[4]) + 1, 0);

	while(fgets(buffer, sizeof(buffer), fp) != NULL){
		int buffer_length = strlen(buffer) + 1;
		send(sock, buffer, buffer_length, 0);
	}
	close(sock);
}
