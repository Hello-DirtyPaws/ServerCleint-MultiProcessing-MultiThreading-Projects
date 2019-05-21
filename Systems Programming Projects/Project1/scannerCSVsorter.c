/*
Authors: Manohar Chitoda & Suraj Upadhyay
Sections: 3 & 2
Date: October 28, 2018
File: scannerCSVsorter.c
*/
//Pre-processors
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "scannerCSVsorter.h"

//Prototypes
void digger(const char *,const char *,char *);
int checkCSV(const char *, int);
int existingCSV(const char *);
void sortCaller(const char *,const char *,char *,char *);

//Global variables
char argument[50] = "-sorted-";
char column[50];
int * counter;
int outDirFixd;
int pid;
int startPID;

//Stating Point of the code
int main(int argc, char *argv[])
{
	int a = 1;
	counter = &a;
	if(argc % 2 == 0)
		{
			printf("Invalid number of arguments passed.\n");
			return -1;
		}
		else
		{
			int counted_c = 0;
			int counted_d = 0;
			int counted_o = 0;
			
			char* col; char* writePath; char* readPath;
			
			int i;
			for(i = 1; i < argc; i = i+2)
			{
				if(strcmp(argv[i],"-c") == 0)
				{
					if(counted_c == 0)
					{
						col = argv[i+1];
						counted_c = 1; //true..-c command used.
					}
					else
					{
						printf("-c passed more than once. Cannot execute!!\n");
						return -1;
					}
				}
				
				else if(strcmp(argv[i],"-d") == 0)
				{
					if(counted_d == 0)
					{
						readPath = argv[i+1];
						counted_d = 1; //true..-d command used.
					}
					else
					{
						printf("-d passed more than once. Cannot execute!!\n");
						return -1;
					}
				}
				
				else if(strcmp(argv[i],"-o") == 0)
				{
					if(counted_o == 0)
					{
						writePath = argv[i+1];
						counted_o = 1; //true..-o command used.
					}
					else
					{
						printf("-o passed more than once. Cannot execute!!\n");
						return -1;
					}
				}
				else
				{
					printf("Invalid argument passed. Valid argumnets are -c, -d, -o\n");
					return -1;
				}
			}//for loop for each command ends here.
			
			if(counted_c == 0)
			{
				printf("Error!!! no -c given. Thus no column name to sort on given.\n");
				return -1;
			}
			if(counted_d == 0)
			{
				readPath = ".";
			}
			if(counted_o == 0)
			{
				outDirFixd = 0; //no fixed out dir given..
				writePath = readPath;
			}
			if(counted_o == 1)
			{
				outDirFixd = 1; //user gave a fixed out dir to store the sorted files..
			}
			
			//Concatenates argument and .csv
			strcat(argument,col);
			strcat(argument, ".csv");
			
			//Stores the PID of the initial process
			startPID = getpid();
			printf("Initial PID: %d\n",startPID);
			
			// PIDs of all the processes
			printf("PIDS of all child processes: ");
			fflush(stdout);
			
			// Call to the driver function
			digger(readPath, writePath, col);
			
			printf("\b\b\nTotal number of processes: %d\n",*counter);
		}//if - else for args to be even or odd ends here.
		
			
		return 0;
}

// Digs through the dirctory to 
void digger(const char * readFrom, const char * writeToPath, char * col)
{
	// Local Variables
	struct dirent *entry;
	int status;
	
	//Directory pointer
	DIR *dir = opendir(readFrom);
	DIR *dirw = opendir(writeToPath);
	//Does this directory exist?
	if(dir == NULL || dirw == NULL)
	{
		if (dirw == NULL)
		{
			fprintf(stderr, "The OUTPUT %s directory does not exist. ", writeToPath);
			return;
		}
		else 
		{
			fprintf(stderr, "The READ %s directory does not exist. ", readFrom);
			return;
		}
	}
	
	
	// read the directory
	while((entry = readdir(dir))!= NULL)
	{
		if(((strcmp(entry->d_name,".") != 0) && (strcmp(entry->d_name,"..") != 0)))
		{
			//What is the entry pointing at?
			switch (entry->d_type)
			{
				// Regular file 
				case DT_REG:
					//determin if it is a csv file!
					pid = fork();	// process to sort the file 
					//Child process
					if (pid == 0) 
					{
						if (checkCSV(entry->d_name, strlen(entry->d_name)) == 0)
						{
							sortCaller(readFrom, writeToPath,col, entry->d_name);
							exit(1);
						}
						
						else 
						{
							exit(1);
						}
					}
					
					//Parent process
					else if (pid > 0) 
					{
						*counter = *(counter)+1;
						int childPid;
						childPid = wait(&status);
						if (childPid > 0) 
						{
							printf("%d, ",childPid);
							fflush(stdout);
						}
					}
					break;
					
				//Directory
				case DT_DIR:
					//Fork a process to check the directory
					pid = fork();
					char path[1024];
					char path2[1024];
					//Do this for the child process
					if (pid == 0) 
					{
						snprintf(path, sizeof(path), "%s/%s",readFrom,entry->d_name);
						if (outDirFixd == 0) 
						{
							snprintf(path2, sizeof(path2), "%s/%s",writeToPath, entry->d_name);
						}
						
						else 
						{
							snprintf(path2, sizeof(path2), "%s",writeToPath);
						}
						
						
						digger(path, path2, col);
					}
					
					//Do this for the parent process
					else if (pid > 0) 
					{
						int childPid;
						childPid = wait(&status);
						*counter = 1 + WEXITSTATUS(status);
						if (childPid > 0) 
						{
							printf("%d, ",childPid);
							fflush(stdout);
						}
					}
					break;
			}//end switch
		}//end if . ..	
	}// end while
	
	// Controls the final 
	if(startPID == getpid())
	{
		closedir(dir);
	}
	else
	{
		closedir(dir);	// Close the directory
		exit(*counter);
	}
}

// checks if a file name is csv extension or 
// if this csv file hasn't already been sorted.
int checkCSV(const char *name, int length)
{
	int i =0;
	int val;
	//loop to check the extension
	for (;i < length;i++) 
	{
		//Is the extension ".csv"?
		if (((strcmp((name+i),".csv")) == 0))
		{
			val = existingCSV(name);
			if (val == 0) 
			{
				return 0;//if yes then return 0
			}
		}
	}
	
	//or else return 1
	return 1;
}

//Checks if a sorted CSV already exists
int existingCSV(const char *name)
{
	int i =0;
	
	//loop to check the extension
	for (;i < strlen(name);i++) 
	{
		//Is the extension ".csv"?
		if ((strcmp((name+i),argument)) == 0)
		{
			return 1;	//if yes then return 1
		}
	}
	
	//or else return 0
	return 0;
}

// Handles only creating the write file path
char * writeFileCreate(char *warray, const char *writePath, char *readFile)
{
	strcpy(warray, writePath);
	strcat(warray, "/");
	
	char temp[100];
	bzero(temp, 100);
	int i;
	
	for (i = 0;i < strlen(readFile);i++)
	{
		if (strcmp((readFile+i), ".csv") == 0) 
		{
			break;
		}
		temp[i] = readFile[i];
	}
	strcat(warray,temp);
	strcat(warray,argument);
	
	return warray;
}

// Controls the building of read and write file path
void sortCaller(const char *readPath, const char *writePath, char * colname, char *readFile)
{
	char writeF[1024];
	char readF[1024];
	const char * writeFile = writeFileCreate(writeF, writePath, readFile);
	
	strcpy(readF, readPath);
	strcat(readF,"/");
	strcat(readF,readFile);
	const char * refile = readF;
	surajIsGreat(refile, writeFile, colname);
	return;
}
//===============================================
//Old code pasted here:
	
int surajIsGreat(const char* readFile, const char *writeFile, char* colName)
{
	FILE* readIn = fopen(readFile, "r");
	if (readIn != NULL)
	{
		HEADING_SIZE = readHeadings(readIn);
		key = isItHere(colName, headingsHead);
		
		if (key >= 0)
		{
		 	//Array to hold the headings in the CSV
			RECORD *first = (RECORD *)malloc(sizeof(RECORD));
			//read data into first
			first = readData(readIn);
			if(first != NULL)
			{
				first = divide(first, getSize(first));
				FILE* writeIn = fopen(writeFile,"w");
				//writefile will always exist with openable permission as it
				//is being created by the program.
				printHeadings(headingsHead, writeIn);
				print(first, writeIn);
				free(first);
				fclose(writeIn);
			}
			else
			{
				fprintf(stderr, "The data in *%s* file is inconsistant.\n", readFile);
			}//end else
		}// end if k >= 0
		
		else
		{
			fprintf(stderr, "The column %s does not exist in %s.\n", colName,readFile);
			fclose(readIn); //Close
	 		return 1;
		}

	}// end if readIn!= Null
	
	
	else
	{
		fprintf(stderr, "%s :No Such File Exists!\n",readFile);
		fclose(readIn); //Close
 		return 1;
	}// Error of file does not exist
	
	fclose(readIn);
	return 0;

}

//Compute if the column name exists in the file
int isItHere(char *arg, Node *x)
{
	Node *y = x;
	int flag = -1;
	int i;
	
	

	for(i = 0;y != NULL;i++)
	{
		
	 	char* str = y->string;
		str[strlen(str)] = '\0';

		if (strcmp(str, arg)==0)
		 {
			flag = i;
			return flag;
		 }
	y = y->next;
	}

	return flag;
}

int readHeadings(FILE* readIn)
{
	int size = 0, i = 0;
	headingsHead = NULL;
	char buffer = fgetc(readIn);

	while(buffer != EOF && buffer != '\n')
		{
			char str[400];
			bzero(str,400);
			size++;
			while(buffer != ',' && buffer != '\n')
			{
				str[i++] = buffer;
				buffer = fgetc(readIn);
			}
		   int k;
		   char* finalStr = (char*)malloc((i)*sizeof(char));
		   bzero(finalStr, i);
			for(k = 0; k < i; k++)
			{
				finalStr[k] = str[k];
			}
			i = 0;
			finalStr[k] = 'F';
			finalStr[k] = '\0';

			Node *newNode = (Node*) malloc(sizeof (Node));
			newNode->next = NULL;
			newNode->string = finalStr;
		
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
		{
			buffer = fgetc(readIn);
		}
			
	}
	return size;
}

RECORD * readData(FILE* readIn)
{
	RECORD *temp, *head;
	temp = head = NULL;
	char buffer = 'A';
	while (buffer != EOF)
	{
		buffer = 0;
		//Allocate space
		RECORD *current = (RECORD *)malloc(sizeof(RECORD));
		current->array = (char**)malloc(HEADING_SIZE * sizeof(char*));

		//Fill the current node with data
		int j = 0, i =1;
		
		char array[400];
		bzero(array, 400);
		for (;buffer != '\n' && i > 0;j++)
		{
			i = 0;
			buffer = fgetc(readIn);
			while (!(buffer == ',' || buffer == '\n') && buffer != EOF)
			{
				if (buffer != '\"')
				{
					array[i++] = buffer;
				}
				else
				{
					int flag = 0;
					while (!(flag == 1 && buffer ==',') && (buffer != '\n'))
					{
					 	array[i++] = buffer;
						buffer = fgetc(readIn);
					  
						if (buffer == '\"')
							flag = 1;
					}
				   break;// brake the outer while loop
				}
				buffer = fgetc(readIn); //Read a character
			 }//end outer while loop

			if (i > 0)
			{	
				if(j >= HEADING_SIZE)
				{
				  //printf("The cols were more than the heading size here when col data is *%s*\n", array); 
				  return NULL;
				}
				// Allow space to be allocated in the heading array
				current->array[j] = (char *)malloc((i) * sizeof(char) + 1);
				int z;
				for(z = 0; z <= i; z++)
				{
					if (z == i)
					{
					 	current->array[j][z] = '\0';
						break;
					}
					current->array[j][z] = array[z];
				}
			}
			else if (i == 0 && buffer != EOF)
			{
			 	i = 1;
			}
	

	}//end row loop
	if(buffer == '\n' && j < HEADING_SIZE)
	{
	  return NULL;
	}

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
	}//end of file
	
	head = NULL;
	head = temp;
	return temp;
}
void print(RECORD *ptr, FILE* writeIn)
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
					fprintf(writeIn, "%s,", y->array[i]);
				}
				else
				{
					fprintf(writeIn, "%s\n", y->array[i]);
				}
			}
			else
			{
				if(i == (HEADING_SIZE-1))
				{
					fprintf(writeIn, "\n");
				}
				else
				{
					fprintf(writeIn, ",");
				}
			}
		 }
		y= y->next;
	}
}

void printHeadings(Node *headings, FILE* writeIn)
{
	Node *l = headings;
	if(l != NULL)
	{
	 	Node * curr = l;
		while (curr->next != NULL)
		{
			fprintf(writeIn, "%s,", curr->string);
			curr = curr->next;
		}
		fprintf(writeIn, "%s\n", curr->string);
	}
}
