#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct msg_dt{
	unsigned int digit : 2;
};

int main(int argc, char *argv){
	int i, j, k;
	char msg_str[1000];

	printf("Enter Message:\n");
	scanf("%s", msg_str);

	int msg_len = strlen(msg_str);

	//checksum_length
	for(i = 0; 1 ; i++){
		if(pow(2,i) >= msg_len+i+1)
			break;			
	}

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
}
