#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define MAX_MSG_LEN 1000

struct msg_dt{
	unsigned int digit : 2;
};

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

	// Necessary Code Here
	int i, j, k;
	char msg_str[1000];

	printf("Enter Message:\n");
	scanf("%s", msg_str);

	int msg_len = strlen(msg_str);

	//checksum_length calculated here
	for(i = 0; 1 ; i++){
		if(pow(2,i) >= msg_len+i+1)
			break;			
	}

	// checksum length
	int cs_len = i;

	struct msg_dt *cs_frame = (struct msg_dt*)calloc((msg_len + cs_len) * sizeof(struct msg_dt), 0);

	//checksum_position_mark
	for(j = cs_len; j >= 0 ; j--){
		cs_frame[(int)pow(2,j) - 1].digit = 3;		
	}

	for(i = 0,j = 0; j < msg_len; i++, j++){
		if(cs_frame[i].digit == 3){
			j--;
			continue;
		}
		else{
			cs_frame[i].digit = (int)(msg_str[j]-'0');
		}	
	}
	
	//display
/*
	for(i = 0; i < cs_len + msg_len ; i++)
		printf("%d\n", cs_frame[i].digit);

	printf("Enter Probability");
*/

	i = 0;
	while(i<=cs_len){
		int inc = pow(2,i);	
		int flag = 0;
		int count = 0;
		for(j = inc-1; j < cs_len + msg_len; j++){
			if(flag == inc){
				flag = 0;
				j += inc-1;
				continue;
			}			
			else{
				flag++;
				if(cs_frame[j].digit == 1){
					count++;				
				}
			}
		}		
		cs_frame[(int)pow(2,i)-1].digit = count%2;
		i++;
	}

	printf("Checksummed Frame: ");
	for(i = 0; i < cs_len + msg_len ; i++)
		printf("%d", cs_frame[i].digit);

	float probability;
	printf("\n\nEnter Probability of error\n");
	scanf("%f", &probability);
	probability = (int)(probability*100+0.5)/100.0;

	srand(time(0));

	int rand_no_1 = (rand() % (100-1+1)) + 1;
	printf("%d\n", rand_no_1);
	if(rand_no_1 <= probability*100){
		// Error
		int rand_no_2 = (rand() % ((cs_len+msg_len-1) - 0 + 1)) + 0;
		// Flip
		cs_frame[rand_no_2].digit = cs_frame[rand_no_2].digit == 0 ? 1 : 0;
		printf("%d\n", rand_no_2);

	}

	for(i = 0; i < cs_len + msg_len ; i++)
		printf("%d   %d\n", i, cs_frame[i].digit);


	// Ends Here

	close(sock);
}