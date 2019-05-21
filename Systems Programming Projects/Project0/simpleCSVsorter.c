/*
Author: Manohar Chitoda
        Suraj Upadhyay
Date: 	09-30-2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "simpleCSVsorter.h"


///////////////////////////////////////////
int isItHere(char *, Node*);//Returns the location of the column to be sorted on.
int readHeadings(); //Read and stores just the heading row and returns the size of the columns.
RECORD* readData(); //Reads and stores all the data in a Linked List and return the head of the list.
void print(RECORD *); // Prints the entire Linked List full of data.
void printHeadings(Node *);//Prints just the Linked List of headings.
Node* headingsHead; // Root Node for storing headings.


int main(int argc, char *argv[])
{
    HEADING_SIZE = readHeadings();
    key = isItHere(argv[2], headingsHead);
    if (key >= 0) 
    {
    	//Array to hold the headings in the CSV
    	RECORD *first = (RECORD *)malloc(sizeof(RECORD));
        
        //read data into first
        first = readData();
        int numNodes = getSize(first);
        first = divide(first, numNodes);
        printHeadings(headingsHead);
        print(first);
    }
    
    else
    {
	 printHeadings(headingsHead);
         printf("\n***************-----------------------****************\n");
         printf("**%s does not exist in the file! Please try another!**\n",argv[2]);
         printf("***************-----------------------****************\n");
    }

	return 0;
}

int isItHere(char *arg, Node *x)
{
    Node *y = x;
    int flag = -1;
    int i;
    
    for(i = 0;y != NULL;i++) 
    {
        char* str = y->string;
        
        str[strlen(arg)] = '\0';
        
        if (strcmp(str, arg)==0)
         {
            flag = i;
            return flag;
         }
        y = y->next;
    }
    
    return flag;
}

int readHeadings()
{
    int size = 0, i = 0;
    headingsHead = NULL;
    char buffer;
    int status = read(STDIN, &buffer, 1);
    while(status > 0 && buffer != '\n')
    {
        char* str = (char*)malloc(1);
        size++;
        while(buffer != ',' && buffer != '\n')
        {
            str[i++] = buffer;
            status = read(STDIN, &buffer, 1);
        }

        i = 0;

        Node *newNode = (Node*) malloc(sizeof (Node));
        newNode->next = NULL;
        newNode->string = str;


        //adding it to the end of the headingLinkedList.
        if (headingsHead == NULL) 
        {
            headingsHead = newNode;
        } 
        else
        {
            Node* curr = headingsHead;
            while (curr->next != NULL) 
            {
                curr = curr->next;
            }
            curr->next = newNode;
        }
        if(buffer != '\n')
            status = read(STDIN,&buffer,1);            
    }
        
    return size;
}

RECORD * readData()
{
    RECORD *temp, *head;
    temp = head = NULL;
    int status = 1;
    //Read to the end of file
    while (status > 0) 
    {
        //Allocate space
        RECORD *current = (RECORD *)malloc(sizeof(RECORD));
        current->array = (char**)malloc(HEADING_SIZE * sizeof(char*));
        
        //Fill the current node with data
        int j = 0, i =1;
        char buffer = (char)0;
        char array[400];
        //Loop that reads the first row in the file
        // and store in an array
        
        for (;buffer != '\n' && i > 0;j++) 
        {
            i =0;
            status = read(STDIN, &buffer, 1);	//Read a character
            // Read the characters
            while (!(buffer == ',' || buffer == '\n') && status > 0) 
            {
                if (buffer != '\"') 
                {
                    array[i] = buffer;
                    i++;
                }
                else 
                {
                    int flag = 0;
                    while (!(flag == 1 && buffer ==',') && (buffer != '\n')) 
                    {
                        array[i] = buffer;
                        i++;
                        status = read(STDIN, &buffer, 1);
                        
                        if (buffer == '\"') 
                            flag = 1;
                    }
                    break;	// brake the loop 
                }
                status = read(STDIN, &buffer, 1);	//Read a character
            }//end inner loop
        
            if (i > 0) 
            {
                char* arr = (char*)malloc(i*sizeof(char));
                int k;
                //Copy the characters in an array of i sized
                for (k = 0;k < i;k++) 
                {
                    arr[k] = array[k];	//point arr to the character in array
                }
                
                // Allow space to be allocated in the heading array
                current->array[j] = (char *)malloc((i) * sizeof(char)+1);
                
                int z;
                //Copy the string in arr to heading array
                for(z = 0; z <= i; z++)
                {
                    if (z == i) 
                    {
                        current->array[j][z] = '\0';
                        break;
                    }
                    current->array[j][z] = arr[z];
                }
            }
            else if (i == 0 && status != 0)
            {
                i = 1;
            }
        
        }//end row loop
        //set proper pointers
        if (temp != NULL) 
        {
            head-> next = current;
            head = current;
        }
        
        else
        {
            temp = head = current;
        }
    }//end file loop
    head = NULL;
    head = temp;
    return temp;
}

void print(RECORD *ptr)
{
    RECORD *y = ptr;

 
    while (y != NULL)
    {
        int i;
        for(i = 0; i < HEADING_SIZE; i++)
        {
            if (y->array[i] != NULL)
            {
                if (i < (HEADING_SIZE-1)) 
                {
                    printf("%s,", y->array[i]);
                }
                else 
                {
                    printf("%s\n",y->array[i]);
                }
            }
            else
            {
                if(i == (HEADING_SIZE-1))
                        printf("\n");
                else
                    printf(",");
            }
        }
        
        y= y->next;
    }
}

void printHeadings(Node *headings)
{
    Node *l = headings;
    if(l != NULL)
    {
        Node * curr = l;
        while (curr->next != NULL)
        {
            printf("%s,", curr->string);
            curr = curr->next;
        }
        printf("%s\n", curr->string);
    }
}