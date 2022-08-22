#include<stdio.h>
#include "prince_version_2_oracle.h"
#include "single_bit_differential_fault_oracle.h"

int main(){

	long cip1 = 0, msg1, cip2;

	msg1 = fault_dec_oracle(cip1, 3, 0);
	cip2 = oracle(msg1);
	
	printf("\nBy adaptive thing, the pairs are: ( %lx, %lx )\n\n", cip1, cip2);
	printf("\nBy normal thing, the pairs are: ( %lx, %lx )\n\n", fault_oracle(msg1, 8 , 0), oracle(msg1));
	}




/*



------------------------------------------------------------------------------------------------------------------------------------------
Fault type	Required no of faults ( n )	Required no of faults using			Round of fault injection	Complexities
						coupon collector problem ( n · H ( n ))
															      Data Time Memory
------------------------------------------------------------------------------------------------------------------------------------------
Integral Fault 	15				50 (approx)					 9.5		      2^4  2^{20} neg
Attack		--------------------------------------------------------------------------------------------------------------------------
	3		15				50 (approx)					 8.5		      2^4  2^{36} neg
------------------------------------------------------------------------------------------------------------------------------------------
Slow Diffusion Fault 	4				8 (approx)					 10		      2^2  2^{22} neg
Attack
	4
------------------------------------------------------------------------------------------------------------------------------------------
Random Bit Fault 	7				1						 10		      2^3  2^{21} 2^3
Attack
	5
------------------------------------------------------------------------------------------------------------------------------------------
	 










Here in our Random Bit Fault Attack model, we inject the fault at the beginning of the 10th round. If the given fault is in the 0th bit, then after one round it affects one sheet and after two rounds, the difference spreads to the whole state without the first full sheet like class 1 in Fig. 20. The difference after two rounds depends upon the bit, where the fault is injected. If the fault is injected in one of \{ 0, 5, 9, 12, 16, 20, 27, 31, 35, 38, 42, 47, 49, 53, 58, 62 \} bits, then the difference after 2 rounds is like the pattern in class 1 of Fig. 20. Similarly, if the fault is injected in one of \{ 1, 6, 10, 13, 17, 21, 24, 28, 32, 39, 43, 44, 50, 54, 59, 63 \}, \{ 2, 7, 11, 14, 18, 22, 25, 29, 33, 36, 40, 45, 51, 55, 56, 60 \}, \{ 3, 4, 8, 15, 19, 23, 26, 30, 34, 37, 41, 46, 48, 52, 57, 61 \} then the pattern after 2 rounds is like the pattern in class 2, 3, 4 respectively. So, by seeing the output after round 11, we can classify the injected fault bits at round 10 into 4 disjoint classes.





color: CCE5FF
























*/
