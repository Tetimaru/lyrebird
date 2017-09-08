/*
Project Lyrebird

Description: This program helps decrypt an encrypted text file as per the 
algorithim provided in the assignment file.
*/
#include "decrypt.h"

/* This function decrypts a character string using the algorithm given
in the assignment details.
PARAM: string = string to be decrypted, stringlen = length of string argument
POST: Returns decrypted string */
char* decrypt(char* string, int stringlen) {
	
	int* numarray = malloc(stringlen*sizeof(int)); /* intermediary */
	if(numarray == NULL)
	{
		printf("Dynamic memory allocation unsuccessful.\n");
		return NULL;
	}
	char* decryptedstring = malloc(stringlen*sizeof(char)); /* final string to return */ 
	if(decryptedstring == NULL)
	{
		printf("Dynamic memory allocation unsuccessful.\n");
		return NULL;
	}
	int i = 0;
	int k = 0;
	int j = 0;
	int l = 0; 
	
	/* setting up the cipher text table */
	int arr[128] = {};
	arr[32] = 0; /* space */
	arr[35] = 27;
	arr[46] = 28;
	arr[44] = 29;
	arr[39] = 30;
	arr[33] = 31;
	arr[63] = 32;
	arr[40] = 33;
	arr[41] = 34;
	arr[45] = 35;
	arr[58] = 36;
	arr[36] = 37;
	arr[47] = 38;
	arr[38] = 39;
	arr[92] = 40;
	for(k = 97; k < 123; k++){ /* characters a-z */
		arr[k] = k-96;
	}
	
	/* Algorithm step 1: */
	for(k = 0; k < stringlen; k++)
	{
		if((k+1)%8 == 0) /* discard every 8th character */
		{
			i++;
		}
		else
		{
			decryptedstring[j] = string[k];
			j++; /* this variable will be used to identify the position in
	decryptedstring */
		}
	}
	stringlen -= i; /* update string length */

	/* Algorithm step 2: */
	unsigned int* cipher = malloc(stringlen/6*sizeof(int)); /* string of cipher numbers */
	if(cipher == NULL)
	{
		printf("Dynamic memory allocation unsuccessful.\n");
		return NULL;
	}

	for(k = 0; k < stringlen; k++)
		numarray[k] = decryptedstring[k]; /* converting tweet to ASCII code */
	
	for(k = 0; k < stringlen; k++)
		numarray[k] = arr[numarray[k]]; /* converting tweet to base 41*/

	for(k = 0; k < stringlen/6; k++) /* grouping six letters */
	{
		cipher[k] = numarray[6*k]*41*41*41*41*41;
		cipher[k] += numarray[6*k+1]*41*41*41*41;
		cipher[k] += numarray[6*k+2]*41*41*41;
		cipher[k] += numarray[6*k+3]*41*41;
		cipher[k] += numarray[6*k+4]*41;
		cipher[k] += numarray[6*k+5];
	}

	/* Algorithm step 3 (using right-to-left binary method): */
	int exponent[31] = {1,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1}; 
	/* binary representation of the exponent 1921821779 */

	unsigned long long int base = 0;
	unsigned long long int modulus = 4294434817;
	j = 0;
	for(k = 0; k < stringlen/6; k++)
	{
		unsigned long long int result = 1; /* The final answer after completing
		step 3 */
		base = cipher[k]%modulus;
		for(i = 30; i >= 0; i--)
		{
			if(exponent[i]%2 == 1)
				result = (result*base)%modulus;

			base = (base*base)%modulus;
		}
		
	/* Algorithm step 4: */
		for(i = 0; i < 6; i++)
		{
			numarray[j+i] = result%41;
			for(l = 32; l < 123; l++)
			{
				if(arr[l] == numarray[j+i])
				{
					decryptedstring[j+5-i] = l; /* converting back to text by 
					iterating over the cipher table to find the inverse */
					break;
				}
			}
			result = result/41;
		}
		j += 6;
	}

	free(numarray);
	free(cipher);
    return decryptedstring;
} 
