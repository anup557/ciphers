#include<stdio.h>
#include "prince_version_2_oracle.h"
#include "prince _v2_fault_oracle.h"

#define round 8
#define nibble_number 0
#define msg_1 0x0123456789abcdef
#define fault_diff 0x0

int main(){	
//	printf("%lx\n",fault_oracle(msg_1,round,nibble_number,fault_diff));
	printf("%lx\n",oracle(msg_1));
	}
	
	
	
	
	
	
	
	
	
	
	
