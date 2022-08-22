#include<stdlib.h>
#include<time.h>

/************************************************: Function's definitions *********************************************************/

long rand_64_bit(void);					// gives 64 bit random number
long* get_msg_list(int nibble_1, int nibble_2, int nibble_3, int nibble_4);	// put all values in given 3-nibbles

void check_all_property_in_64_bit(long* msg_list, int msg_list_size, long all_position_number);
								// depending upon given all_position_number the function checks whether the 									   following list have all property in those specific positions & returns 0 									   or 1 depending upon that

void check_balanced(long* cipher_list, long limit);		// checks whether the list is balanced or not
void print_matrix_form(long msg);				//print 64-bit msg in matrix form



/****************************************************: Functions :*******************************************************************/
long rand_64_bit(void){
//	srand(time(0));					// call srand from main function
		
	int a,b;
	long c = 0;
	a = rand()&0xffffffff;
	b = rand()&0xffffffff;

	c = ((long)a<<32)|b;
	return c;
	}


long* get_msg_list(int nibble_1, int nibble_2, int nibble_3, int nibble_4){
	long *msg_list, limit = (1<<16);
	msg_list = (long*)malloc(limit*sizeof(long));
	
	for(long i=0; i<limit; i++)
		msg_list[i] = ((i&0xf)<<(60 - 4*nibble_1))|(((i>>4)&0xf)<<(60 - 4*nibble_2))|(((i>>8)&0xf)<<(60 - 4*nibble_3))|(((i>>12)&0xf)<<(60 - 4*nibble_4));

	
	return msg_list;

	}



void check_all_property_in_64_bit(long* msg_list, int msg_list_size, long all_position_number){
	int bit_count = 0, count = 0, position_in_ctr = 0;
	for(int i=0;i<64;i++)
		if((all_position_number>>i)&1 == 1)	
			bit_count++;					// extracting number of 1's from all_position_number
	
	long ctr_array_size = (1<<bit_count);
	
	int* ctr, *bit_position;
	bit_position = (int*)malloc(bit_count*sizeof(int));
	ctr = (int*)malloc(ctr_array_size*sizeof(int));

	for(long i=0;i<ctr_array_size;i++)
		ctr[i] = 0;						// initializing ctr to 0

	for(int i=0;i<64;i++)
		if((all_position_number>>i)&1 == 1)
			bit_position[count++] = i;			// storing the positions, where we have to check all
			
		
	for(int i=0;i<msg_list_size;i++){
		position_in_ctr = 0;
		for(int j=0; j<bit_count; j++)
			position_in_ctr = (position_in_ctr<<1) | ((msg_list[i]>>bit_position[j])&1);
		ctr[position_in_ctr]++;
		}
	
	int ctr_count = 0;
	for(long i=0;i<ctr_array_size;i++)
		if(ctr[i] == 0){
			ctr_count = 1;
			break;
			}
	if(ctr_count == 1)
		printf("not all.\n");
	else 
		printf("all.\n");	
	}
	
	
void check_balanced(long* cipher_list, long limit){
	long balance = 0;	
	for(long i=0;i<limit;i++)
		balance = balance^cipher_list[i];
	if(balance == 0)
		printf("balanced.\n");
	else 
		printf("Not balanced.\n");	
	}


	
void print_matrix_form(long msg){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			printf("%lx  ",(msg>>60-(4*(4*j + i)))&0xf);	
		printf("\n");
		}	
	printf("\n");	
	}
	

