/*
Project Lyrebird

Description: This program decrypts an encrypted text file using
the following decryption algorithm:
1) Remove extra symbols added every 8th character
2) Transform every group of 6 characters into a base 41 integer C
3) Apply the transformation M = C^d mod n where d is  1921821779 and n is 4294434817
4) Apply the inverse of step 2 to M
*/
#ifndef _DECRYPT_H_
#define _DECRYPT_H_

#include <string.h>
#include <stdio.h>
#include "memwatch.h"

/* Sets up and returns the cipher and plain text code table 
POST: returns the code table */
int* getCipherTable();

/* This function removes every 8th character from string orig and puts the result in string newstr 
PARAM: orig = the string to prune
PARAM: newstr = the buffer to place the pruned string
PARAM: stringlen = the length, in integer, of orig
POST: returns the new stringlen after pruning (stringlen of newstr) */
int prune(char* orig, char* newstr, int stringlen);

/* This function decrypts a character string of length stringlen.
PARAM: string = string to be decrypted
PARAM: stringlen = length of string argument
POST: Returns decrypted string */
void decrypt(char* string, int stringlen, int* code_table, unsigned int* cipher, unsigned int* buf_int);

#endif 

