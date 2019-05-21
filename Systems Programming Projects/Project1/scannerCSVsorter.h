/*
Authors: Manohar Chitoda & Suraj Upadhyay
Sections: 3 & 2
Date: October 28, 2018
File: scannerCSVsorter.h
*/

#ifndef SCANNERCSVSORTER_H
#define SCANNERCSVSORTER_H
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


//SIMPLECSVSORTER.C FUNCTIONS
///////////////////////////////////////////
int isItHere(char *, Node*);//Returns the location of the column to be sorted on.
int readHeadings(FILE*); //Read and stores just the heading row and returns the size of the columns.
RECORD* readData(FILE*); //Reads and stores all the data in a Linked List and return the head of the list.
void print(RECORD *, FILE*); // Prints the entire Linked List full of data.
void printHeadings(Node *, FILE*);//Prints just the Linked List of headings.
Node* headingsHead; // Root Node for storing headings.
int surajIsGreat(const char *,const char *,char *);//Takes in the reading file name and the col name to sort on.
///////////////////////////////////////////



#endif /* SCANNERCSVSORTER_H */
