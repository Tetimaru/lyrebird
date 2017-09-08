/*
Project Lyrebird

Description: This program helps decrypt an encrypted text file as per the 
algorithim provided in the assignment file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decrypt.h"
#include "memwatch.h"

int main (int argc, char **argv) {

	static const int maxlen = 170;
	FILE* inFile = fopen(argv[1], "r");
	if (inFile == NULL) //file open error checking
	{
	    printf("Error opening input file!\n");
	    return -1;
	}
	FILE* outFile = fopen(argv[2], "w");
	if (outFile == NULL) //file open error checking
	{
	    printf("Error opening output file!\n");
	    return -1;
	}
	char* encryptedtweet = malloc(maxlen*sizeof(char));
	char* decryptedtweet;
	int j = 0;
	int len = 0; 

	while(!feof(inFile))
	{
		fgets(encryptedtweet, maxlen, inFile); /* read a line from the encrypted file */
		len = strlen(encryptedtweet); /* length of the encrypted tweet */
		
		/*decrypt the encrypted tweet*/
		decryptedtweet = decrypt(encryptedtweet, len);
		if(decryptedtweet == NULL)
		{
			printf("Attempting to decrypt nonexistent data. Terminating.\n");
			return -2;
		}
		/*print the decrypted tweet to the specified output file*/
		for(j = 0; j < len-len/8; j++)
		{
			if(decryptedtweet[j] != '\n')
				fprintf(outFile, "%c", decryptedtweet[j]);
		}
		fprintf(outFile, "\n");
		
		/*clear encryptedtweet buffer so that it can be reused*/
		memset(encryptedtweet, 0, maxlen);
		free(decryptedtweet);

	}
	
	free(encryptedtweet);
	if(!(!fclose(inFile))) //file close error checking
	{
		printf("File close error.\n");
		return -3;
	}
	if(!(!fclose(outFile))) //file close error checking
	{
		printf("File close error.\n");
		return -3;
	}
	
	return 0;
} 


