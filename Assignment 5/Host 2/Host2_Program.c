#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<math.h>
#include<time.h>

#define MAX_CLIENT 1
#define Server_Port 17002
#define MAX_MSG_LEN 1000

struct msg_dt{
	unsigned int digit : 2;
};

int main(int argc, char *argv[]){
	int serv_sock, new_serv_sock;
	
	int client_addr_len, recv_data_len;

	if(argc < 3){
		printf("usage: %s <IP Address> <Port>", argv[0]);
		exit(1);
	}

	struct sockaddr_in client_addr, server_addr;
	char buffer[MAX_MSG_LEN];
	
	bzero((char*)&server_addr, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_sock < 0){
		perror("Cannot open socket\n");
		exit(1);
	}

	if(bind(serv_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		perror("Cannot Bind Port");
		exit(1);
	}

	listen(serv_sock, MAX_CLIENT);

	while(1){
		client_addr_len = sizeof(client_addr);
		new_serv_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &client_addr_len);
		if(new_serv_sock < 0){
			perror("Cannot accept connection");
			exit(1);
		}

		// Necessary Code Here

		close(new_serv_sock);
	}
}