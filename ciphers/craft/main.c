#include<stdio.h>
#include "craft_oracle.h"
int main(){
	long msg, k0, k1, tweak, cip;
	printf("\nenter 64-bit plaintext:");
	scanf("%lx",&msg);
	
	printf("\nenter 64-bit k0:");
	scanf("%lx",&k0);
	
	printf("\nenter 64-bit k1:");
	scanf("%lx",&k1);
	
	printf("\nenter 64-bit tweak:");
	scanf("%lx",&tweak);


	cip = oracle(msg, k0, k1, tweak);
	printf("\n\ncip is: %lx\n\n",cip);	
	
	}
