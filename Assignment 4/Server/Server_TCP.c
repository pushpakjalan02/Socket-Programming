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

void childProcess(int new_serv_sock){
	if(new_serv_sock < 0){
			perror("Cannot accept connection");
			exit(1);
	}

	char buffer[MAX_MSG_LEN];

	memset(buffer, 0x0, MAX_MSG_LEN);

	recv(new_serv_sock, buffer, sizeof(buffer), 0);

	int i = 0;

	while(i < strlen(buffer)/2){
		char temp = buffer[i];
		buffer[i] = buffer[strlen(buffer) - i - 1];
		buffer[strlen(buffer)-i-1] = temp;
		i++;
	}

	send(new_serv_sock, buffer, strlen(buffer) + 1, 0);

	close(new_serv_sock);

}

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
		
		int pid;

		if((pid = fork()) == 0){
			close(serv_sock);
			childProcess(new_serv_sock);
			exit(0);
		}

		close(new_serv_sock);
	}

	close(serv_sock);
}