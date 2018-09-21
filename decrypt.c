/*
Project Lyrebird

Description: This program decrypts an encrypted text file using
the following decryption algorithm:
1) Remove extra symbols added every 8th character
2) Transform every group of 6 characters into a base 41 integer C
3) Apply the transformation M = C^d mod n where d is  1921821779 and n is 4294434817
4) Apply the inverse of step 2 to M
*/
#include "decrypt.h"

/* Sets up and returns the cipher and plain text code table 
POST: returns the code table */
int* getCipherTable() {
	int* codeTable = malloc(128 * sizeof(int)); 
	codeTable[32] = 0; /* index = ASCII code */
	codeTable[35] = 27;
	codeTable[46] = 28;
	codeTable[44] = 29;
	codeTable[39] = 30;
	codeTable[33] = 31;
	codeTable[63] = 32;
	codeTable[40] = 33;
	codeTable[41] = 34;
	codeTable[45] = 35;
	codeTable[58] = 36;
	codeTable[36] = 37;
	codeTable[47] = 38;
	codeTable[38] = 39;
	codeTable[92] = 40;
    int i = 0;
	for(i = 97; i < 123; i++){ /* characters a-z */
		codeTable[i] = i-96;
	}
    return codeTable;
}

/* This function removes every 8th character from string orig and puts the result in string newstr 
PARAM: orig = the string to prune
PARAM: newstr = the buffer to place the pruned string
PARAM: stringlen = the length, in integer, of orig
POST: returns the new stringlen after pruning (stringlen of newstr) */
int prune(char* orig, char* newstr, int stringlen) {
    int shorten = 0;
	int loc = 0; /* this variable will be used to identify the position in newstr */
    int i = 0;
	for(i = 0; i < stringlen; i++)
	{
		if((i+1)%8 == 0) /* discard every 8th character */
			shorten++;
		else
		{
			newstr[loc] = orig[i];
			loc++; 
		}
	}
	stringlen -= shorten; /* update string length */
    return stringlen;
}  

/* This function decrypts a character string of length stringlen.
PARAM: string = string to be decrypted
PARAM: stringlen = length of string argument
POST: Returns decrypted string */
void decrypt(char* string, int stringlen, int* code_table, unsigned int* cipher, unsigned int* buf_int) {
    
	int i = 0; /* loop counter */

	/* Algorithm step 2: Transform every group of 6 characters into a base 41 integer C */
	for(i = 0; i < stringlen; i++)
		buf_int[i] = string[i]; /* converting text to ASCII */
	
	for(i = 0; i < stringlen; i++)
		buf_int[i] = code_table[buf_int[i]]; /* converting tweet to base 41*/

	for(i = 0; i < stringlen/6; i++) /* grouping six letters */
	{
		cipher[i] = buf_int[6*i]*41*41*41*41*41;
		cipher[i] += buf_int[6*i+1]*41*41*41*41;
		cipher[i] += buf_int[6*i+2]*41*41*41;
		cipher[i] += buf_int[6*i+3]*41*41;
		cipher[i] += buf_int[6*i+4]*41;
		cipher[i] += buf_int[6*i+5];
	}

	/* Algorithm step 3: Apply the transformation M = C^d mod n where d is 1921821779 and n is 4294434817 (using right-to-left binary method): */
	int exponent[31] = {1,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1}; 
	/* binary representation of the exponent 1921821779 */

	unsigned long long int base = 0;
	unsigned long long int modulus = 4294434817;
	int j = 0;
    int k = 0; /* loop counter */
    int l = 0;  /* loop counter */
    
	for(k = 0; k < stringlen/6; k++)
	{
		unsigned long long int result = 1; /* The final answer after completing step 3 */
		base = cipher[k] % modulus;
		for(i = 30; i >= 0; i--)
		{
			if(exponent[i] % 2 == 1) /* 1 */
				result = (result*base) % modulus;

			base = (base*base) % modulus;
		}
		
        /* Algorithm step 4: Apply the inverse of step 2 to M */
		for(i = 0; i < 6; i++)
		{
			buf_int[j+i] = result%41;
			for(l = 32; l < 123; l++)
			{
				if(code_table[l] == buf_int[j+i])
				{
					string[j+5-i] = l; /* converting back to text by 
					iterating over the cipher table to find the inverse */
					break;
				}
			}
			result = result/41;
		}
		j += 6;
	}
} 
