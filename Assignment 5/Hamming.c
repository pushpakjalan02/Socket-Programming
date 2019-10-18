#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

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
	// Transmit
	

	//Check
	int cs_frame_length = cs_len + msg_len;
	i = 0;
	j = 0;
	k = 0;
	while(1){
		if(pow(2,i) >=  cs_frame_length + 1)
			break;
		i++;
	}
	cs_len = i;
	msg_len = cs_frame_length - cs_len;
	i = 0;
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
	cs_frame[sum - 1].digit = cs_frame[sum - 1].digit == 1 ? 0 : 1;
/*
	for(i = 0; i < cs_len + msg_len ; i++)
		printf("%d   %d\n", i, cs_frame[i].digit);
*/
	i = 0;
	j = 0;
	printf("\n\nMessage is:");
	while(i < cs_frame_length){
		i++;
		if(pow(2, j) == i){
			j++;
			continue;
		}
		printf("%d", cs_frame[i-1].digit);
	}
	printf("\n");
}























