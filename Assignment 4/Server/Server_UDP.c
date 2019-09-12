#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

#define MAX_CLIENT 5
#define Server_Port 17002
#define MAX_MSG_LEN 1000


int main(int argc, char *argv[]){
	int serv_sock;
	
	int client_addr_len;
	
	char buffer[MAX_MSG_LEN];

	if(argc < 3){
		printf("usage: %s <IP Address> <Port>", argv[0]);
		exit(1);
	}

	struct sockaddr_in client_addr, server_addr;
	
	bzero((char*)&server_addr, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(serv_sock < 0){
		perror("Cannot open socket\n");
		exit(1);
	}

	if(bind(serv_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		perror("Cannot Bind Port");
		exit(1);
	}

	while(1){
		memset(buffer, 0x0, MAX_MSG_LEN);		
		
		memset(&client_addr, 0, sizeof(client_addr));
		
		client_addr_len = sizeof(client_addr);
		
		recvfrom(serv_sock, (char*) buffer, MAX_MSG_LEN, MSG_WAITALL, (struct sockaddr*) &client_addr, &client_addr_len);

		int i = 0;

		while(i < strlen(buffer)/2){
			char temp = buffer[i];
			buffer[i] = buffer[strlen(buffer) - i - 1];
			buffer[strlen(buffer)-i-1] = temp;
			i++;
		}


		sendto(serv_sock, (const char*)buffer, strlen(buffer) + 1, MSG_CONFIRM, (struct sockaddr*) &client_addr, (socklen_t)client_addr_len);
		
	}

	close(serv_sock);
}
