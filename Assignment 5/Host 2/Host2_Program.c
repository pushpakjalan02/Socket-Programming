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
		printf("\n\n------------------------------------------------------\n\n");
		memset(buffer, 0x0, MAX_MSG_LEN);
		recv(new_serv_sock, buffer, sizeof(buffer), 0);
		printf("Received Frame: %s\n", buffer);
		int cs_frame_length = strlen(buffer);

		int i = 0;
		int j = 0;
		int k = 0;
		
		// Build Structure
		struct msg_dt *cs_frame = (struct msg_dt*) calloc(sizeof(struct msg_dt) * cs_frame_length, 0);
		for(i = 0; i < cs_frame_length; i++)
			cs_frame[i].digit = (int)(buffer[i] - '0');

/*
		for(i = 0; i < cs_frame_length ; i++)
			printf("%d   %d\n", i, cs_frame[i].digit);
*/

		// Calculate length of checksum from total length and save it.
		while(1){
			if(pow(2,i) >=  cs_frame_length + 1)
				break;
			i++;
		}
		int cs_len = i;

		// Length of message only.
		int msg_len = cs_frame_length - cs_len;
		
		i = 0;
		
		// Error Correction Code Here
		int sum = 0;
		while(i < cs_len){
			int checksum_position = pow(2, i) - 1;
			int interval = pow(2, i);
			int count = 0;
			int flag = 0;
			for(j = checksum_position; j < cs_frame_length; j++){
				flag++;
				if(cs_frame[j].digit == 1)
					count++;
				if(flag == interval){
					flag = 0;
					j += interval;
					continue;
				}
			}
			if(count%2 == 1){
				sum += pow(2, i);	
			}
			i++;
		}

		// Flip the error bit if error present
		if(sum > 0)
			cs_frame[sum - 1].digit = cs_frame[sum - 1].digit == 1 ? 0 : 1;

		// Display Received Message After Correction
		printf("Received Frame after Correction: ");
		for(i = 0; i < cs_len + msg_len ; i++)
			printf("%d", cs_frame[i].digit);
		printf("\n");

		// Display Corrected Message only
		i = 0;
		j = 0;
		printf("\nMessage is: ");
		while(i < cs_frame_length){
			i++;
			if(pow(2, j) == i){
				j++;
				continue;
			}
			printf("%d", cs_frame[i-1].digit);
		}
		printf("\n");

		// Ends Here
		
		close(new_serv_sock);

		printf("\n------------------------------------------------------\n\n");
	}
}