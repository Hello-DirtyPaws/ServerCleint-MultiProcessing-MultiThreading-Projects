/*
Author: Manohar Chitoda
		Suraj Upadhyay
Date: 	09-30-2018
*/
#ifndef SIMPLECSVSORTER_H
#define SIMPLECSVSORTER_H
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct rec
{
	char** array;
	struct rec *next;
}RECORD;

typedef struct node
{
	char *string;
	struct node *next;
}Node;


//MERGE SORT CONDUCTORS
RECORD* divide(RECORD*, int); //Splits the list in half using recursion, executes merge and returns the head of the sorted list. 
int getSize(RECORD*);//Returns the size of any linked list from the RECORD * node passed.
RECORD* merge(RECORD*, RECORD*, RECORD*);//Merges two lists(1 and 2) and puts them into the sorted list(3).
RECORD* addLast(RECORD*, RECORD*);//Adds the new node(2) in the end of the list node(1)




//STRING MANIPULATORS
char* trim(char*);//Trims the string in correct format..removes unwanted trailing spaces keeping the original data unchanged.
int isDigit(char*);//Returns 1 if the string is a digit otherwise 0.
int compareTo(char*, char*);//Compares 2 strings accordingly as strings or as numbers 




int HEADING_SIZE;
int key;


#endif /* SIMPLECSVSORTER_H */

