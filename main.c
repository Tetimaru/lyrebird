/*
Project Lyrebird

Description: This program decrypts an encrypted text file using
the following decryption algorithm:
1) Remove extra symbols added every 8th character
2) Transform every group of 6 characters into a base 41 integer C
3) Apply the transformation M = C^d mod n where d is  1921821779 and n is 4294434817
4) Apply the inverse of step 2 to M
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "decrypt.h"
#include "memwatch.h"

void checkMemoryAllocation(void* buf) {
    if(buf == NULL) {
		printf("Dynamic memory allocation unsuccessful.\n");
		exit(EXIT_FAILURE);
	}
}
int main (int argc, char **argv) {
    // constants
	static const int MAX_LEN = 170;
    char helpMsg[64];
    snprintf(helpMsg, sizeof(helpMsg), "Usage: %s encrypted_tweets.txt output.txt", argv[0]);
    
    if (argc < 3) { // must have at least 2 arguments
        printf("%s\n", helpMsg);
        return -1;
    }
    // get encrypted tweets file
	FILE* inFile = fopen(argv[1], "r");
	if (inFile == NULL) // input file open error checking
	{
	    printf("Error opening encrypted tweets input file!\n");
        printf("%s\n", helpMsg);
	    return -1;
	}
    // get output file
	FILE* outFile = fopen(argv[2], "w");
	if (outFile == NULL) // output file open error checking
	{
        printf("Error opening output file!\n");
        printf("%s\n", helpMsg);
	    return -1;
	}
    
    /* Allocate buffers */
	char* encryptedTweet = malloc(MAX_LEN * sizeof(char));
    checkMemoryAllocation(encryptedTweet);
    
    char* intermediary_char = malloc(MAX_LEN * sizeof(char)); /* buffer for storing intermediate results */
    checkMemoryAllocation(intermediary_char);
    unsigned int* intermediary_uint = malloc(MAX_LEN * sizeof(unsigned int)); /* buffer for storing intermediate results */
    checkMemoryAllocation(intermediary_uint);
    
    int* codeTable = getCipherTable(); /* get code translation table */
    // int i = 0;
    // for(i = 0; i < 128; i++) {
        // printf("%d: %d\n", i, codeTable[i]);
    // }
    
    unsigned int* cipher = malloc((MAX_LEN/6)*sizeof(unsigned int)); /* string of cipher numbers */
    checkMemoryAllocation(cipher);

	while(!feof(inFile))
	{
		fgets(encryptedTweet, MAX_LEN, inFile); /* read a line from the encrypted file */
		int len = strlen(encryptedTweet); /* length of the encrypted tweet */
		
        /* Algorithm step 1: Remove extra symbols added every 8th character */
        len = prune(encryptedTweet, intermediary_char, len); 
		/* Algorithm steps 2-4: Decrypt the encrypted tweet */
        decrypt(intermediary_char, len, codeTable, cipher, intermediary_uint);
        //assert(len == strlen(intermediary_char));
        int i = 0; /* loop counter */
		/*print the decrypted tweet to the specified output file*/
		for(i = 0; i < len; i++) /* removed every 8th character */
		{
			if(intermediary_char[i] != '\n')
				fprintf(outFile, "%c", intermediary_char[i]);
		}
		fprintf(outFile, "\n");
		
		/*clear encryptedTweet buffer so that it can be reused*/
		memset(encryptedTweet, 0, MAX_LEN);
        memset(intermediary_char, 0, MAX_LEN);
        memset(intermediary_uint, 0, MAX_LEN);
        memset(cipher, 0, MAX_LEN/6);
	}
	
	free(encryptedTweet);
    free(intermediary_char);
    free(intermediary_uint);
    free(codeTable);
    free(cipher);
    
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


