#include<stdio.h>
#include<time.h>
#include "AES_common.c"

void long_to_word8(word8 new_msg[][4], long msg){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			new_msg[i][j] = (msg>>(60 - 16*i - 4*j))&0xf;
	}
	
void word8_to_long(word8 msg[][4], long* new_msg){
	*new_msg = 0;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			*new_msg = (*new_msg<<4)|(msg[i][j]&0xf);
	}
	
long rand_64_bit(void){
//	srand(time(0));					// call srand from main function
		
	int a,b;
	long c = 0;
	a = rand()&0xffffffff;
	b = rand()&0xffffffff;

	c = ((long)a<<32)|b;
	return c;
	}
	
void xor(word8 msg1[][4],word8 msg2[][4]){
	word8 diff[4][4];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			diff[i][j] = msg1[i][j]^msg2[i][j];
	printtt(diff);			
	}
		

int main(){
	srand(time(0));

	word8 alpha[4][4] = {{0x2,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, msg1[4][4], msg2[4][4], beta[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, key[4][4] = {{0x0,0x1,0x2,0x3},{0x4,0x5,0x6,0x7},{0x8,0x9,0xa,0xb},{0xc,0xd,0xe,0xf}}, delta[4][4] = {{0x1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	
	word8 cip1[4][4], cip2[4][4], cip3[4][4], cip4[4][4], pre_cip1[4][4], pre_cip2[4][4], post_cip3[4][4], post_cip4[4][4], msg3[4][4], msg4[4][4];

	int limit = 1<<2;	
	for(int i=0;i<limit;i++){
		long msg;
		long_to_word8(msg1, rand_64_bit());
		
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				msg2[i][j] = msg1[i][j]^alpha[i][j];		// giving alpha difference

//xor(msg1, msg2);				
		encryption(msg1, key, pre_cip1);				// pre_cip1 = E0(msg1)
		encryption(msg2, key, pre_cip2);				// pre_cip2 = E0(msg2) 
//xor(pre_cip1,pre_cip2);
//printf("\n\n");
		
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				beta[i][j] = pre_cip1[i][j]^pre_cip2[i][j];
		
		encryption(pre_cip1, key, cip1);				// cip1 = E0.E1(msg1)
		encryption(pre_cip2, key, cip2);				// cip2 = E0.E1(msg2)

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				cip3[i][j] = cip1[i][j]^delta[i][j];		// xor-ing cip1 & cip2 with delta
				cip4[i][j] = cip2[i][j]^delta[i][j];
				}

		decryption(cip3, key, post_cip3);
		decryption(cip4, key, post_cip4);

//xor(post_cip3, post_cip4);
		decryption(post_cip3, key, msg3);
		decryption(post_cip4, key, msg4);

		int ctr = 1;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				if((msg3[i][j]^msg4[i][j])!=alpha[i][j])
					ctr = 0;

		if(ctr == 1){
			printf("\nboomerang successful.\n");
			}
		else 	
			printf("\nboomerang unsuccessful.\n");
		}
	



	}
