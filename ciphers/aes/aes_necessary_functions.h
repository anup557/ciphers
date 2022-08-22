#include<stdio.h>
#include<stdlib.h>

long rand_64_bit(void){
//	srand(time(0));					// call srand from main function
		
	int a,b;
	long c = 0;
	a = rand()&0xffffffff;
	b = rand()&0xffffffff;

	c = ((long)a<<32)|b;
	return c;
	}

void print_matrix_form(long msg){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			printf("%lx  ",(msg>> 60-(4*(4*j + i)))&0xf);	
		printf("\n");
		}	
	printf("\n");	
	}
	

