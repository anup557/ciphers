#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define sbox_side_len 1
#define gap 0.2
#define round_gap 5

#define rounds 4

/* int perm[64] = {	0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51, 		// original PRESENT cipher perm */
/* 		 	4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55, */
/* 		 	8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59, */
/* 			12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63}; */

int perm[64] = {   0, 17, 34, 51, 48, 1, 18, 35, 32, 49, 2, 19, 16, 33, 50, 3,                                                                                                           4, 21, 38, 55, 52, 5, 22, 39, 36, 53, 6, 23, 20, 37, 54, 7,                                                                                                           8, 25, 42, 59, 56, 9, 26, 43, 40, 57, 10, 27, 24, 41, 58, 11,                                                                                                        12, 29, 46, 63, 60, 13, 30, 47, 44, 61, 14, 31, 28, 45, 62, 15};	// GIFT cipher perm

void generate_latex_file(char* name){
	FILE *fp;
	
	strcat(name, ".tex");	
	fp = fopen(name, "a+");

	float line_pos[64];
	
	float delta = ((float)sbox_side_len)/10;
	float sbox_line_dist = (((float)sbox_side_len)/3) - (2*((float)delta)/3);	

	fprintf(fp,"\n	\\documentclass[12pt,a4paper ]{report}					");
	fprintf(fp,"\n	\\usepackage[a4paper,total={7.5in,10in}]{geometry}			");
	fprintf(fp,"\n	\\usepackage{amsmath,amsthm}						");
	fprintf(fp,"\n	\\usepackage{tikz}							");
	fprintf(fp,"\n										");
	fprintf(fp,"\n	\\begin{document}							");
	fprintf(fp,"\n	\\begin{center}								");
	fprintf(fp,"\n										");
	fprintf(fp,"\n	\\begin{tikzpicture}							");

	for(int i=0;i<rounds;i++){
		for(int sbox_num = 0; sbox_num<16; sbox_num++){
			fprintf(fp,"\n	\\node (rect) at (%d + %d*%f, -%d) [draw,thick,minimum width=%d cm,minimum height=%d cm]{S}; ", 									sbox_num, sbox_num, gap,i*round_gap, sbox_side_len, sbox_side_len);
			for(int bit_line = 0; bit_line<4; bit_line++){
				line_pos[4*sbox_num + bit_line] = - ((float)sbox_side_len/2) + sbox_num + (sbox_num*gap) + delta + 										(bit_line*sbox_line_dist);
// giving xor 
				fprintf(fp, "\\node at (%f, %d)[minimum width=%d cm,minimum height=%d cm]{$\\oplus$};", line_pos[4*sbox_num 										+ bit_line],- i*round_gap + sbox_side_len, sbox_side_len, 										sbox_side_len);

// input lines 		
				fprintf(fp,"\n	\\draw[->, gray, very thin] (%f, %f) -- (%f, %f); ", line_pos[4*sbox_num + bit_line], - i*round_gap + 									3*(float)sbox_side_len/2 , line_pos[4*sbox_num + bit_line], - i*round_gap + 									(float)sbox_side_len/2);
// output lines
				fprintf(fp,"\n	\\draw[gray, very thin] (%f, -%f) -- (%f, -%f); ", line_pos[4*sbox_num + bit_line], i*round_gap + 									(float)sbox_side_len/2, line_pos[4*sbox_num + bit_line], i*round_gap + 									(float)sbox_side_len);
				
				}
			}
		
		for(int j=0;j<64;j++)
			fprintf(fp,"\n	\\draw[gray, very thin] (%f, - %d) -- (%f, -%f);",line_pos[j], i*round_gap + sbox_side_len, line_pos[perm[j]], 									(i+1)*round_gap - (float)sbox_side_len*3/2);
		}
	
	for(int temp_bit_line=0; temp_bit_line<64; temp_bit_line++){
		fprintf(fp,"\n	\\draw[->, gray, very thin] (%f, %f) -- (%f, %f); ", line_pos[temp_bit_line],- rounds*round_gap + 3*(float)sbox_side_len/2, 									line_pos[temp_bit_line],  - rounds*round_gap + (float)sbox_side_len/2);
		fprintf(fp, "\\node at (%f, %d)[minimum width=%d cm,minimum height=%d cm]{$\\oplus$};", line_pos[temp_bit_line],- 									rounds*round_gap + sbox_side_len, sbox_side_len, sbox_side_len);
		}


	fprintf(fp,"\n	\\end{tikzpicture}							");
	fprintf(fp,"\n	\\end{center}								");
	fprintf(fp,"\n	\\end{document}								");
	fprintf(fp,"\n										");			
	}


int main(int argc, char *argv[]){
	generate_latex_file(argv[1]);	

	printf("\n");
	}











