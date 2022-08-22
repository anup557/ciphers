#include<stdio.h>
#include "small_aes_oracle.h"
#include "inv_small_aes_oracle.h"
#include "small_aes_necessary_functions.h"

int main(){
	int msg = 2;
	
	for(int i=0;i<7;i++)
		msg = mult_2_field[msg];		
	printf("%x\n",msg);
	}
	
