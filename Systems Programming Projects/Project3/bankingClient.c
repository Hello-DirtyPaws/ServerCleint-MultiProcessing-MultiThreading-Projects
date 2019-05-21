//Client
//Authors: Manohar Chitoda & Suraj Upadhyay
//Asst: Asst3
//Date: December 12, 2018
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#include <pthread.h>
#include <signal.h>


pthread_t t_read = NULL, t_write = NULL; 
int clientSocket;

int createConnection(char*,int);
void* readNPrint(void*);
void* sendToProcess(void*);
int inputChecker(char* );
int commandHandler(char*, char*);
void termination();
int isDigit(char*);

int main(int argc, char* argv[]) 
{
  if(argc != 3)
  {
      fprintf(stderr, "Invalid arguments.\n");
      return -1;
  }
  char* ip = argv[1];
  char * str = argv[2];
    
  //char * ip = "Surajs-MacBook-Pro-2.local";
  //char * str = "26196";
  
  
  if(isDigit(str) == 0)
  {
      fprintf(stderr, "ERROR! Invalid port number provided.\n");
      return -1;
  }
  int port = atoi(str);
  if(port < 8192)
  {
      fprintf(stderr, "ERROR! Please choose a port number > 8192 to be on a safe side.\n");
      return -1;
  }
  
  signal(SIGINT, termination);
  
  createConnection(ip, port);
  return 0;
}

int createConnection(char *ip, int port)
{
  struct sockaddr_in serverAddr;
  
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serverAddr , '0', sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port=htons(port);

  struct hostent *hp = gethostbyname(ip);
  
  bcopy((char*)hp->h_addr, (char*)&serverAddr.sin_addr, hp->h_length);

  int isConnected = -1;
  while(isConnected != 0)
  {
    isConnected = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(isConnected == 0)
    {
      printf("CONNECTED\n");
    }
    else
    {
      //could not connect. Try again after 3 sec.
      sleep(3);
    }
  }
  pthread_create(&t_read, NULL, readNPrint, &clientSocket);
  pthread_create(&t_write, NULL, sendToProcess, &clientSocket);
  pthread_join(t_read, NULL);
  pthread_join(t_write, NULL);
  
  close(clientSocket);
  return 0;
}

//This thread function will just read from the serve and
//print it to the console as it is. That's it.
void* readNPrint(void* clientSock)
{
  int clientSocket = *((int*)clientSock);
  char buffer[1024];
  int i = 0;
  while(i == 0)
  {
    bzero(buffer, 1024);
    int dataRead = read(clientSocket, buffer, 700);
    if(dataRead > 0)
    {
      printf("%s\n", buffer);
      fflush(stdout);
      if(strcmp(buffer, "Bank server is shutting down. Sorry for the inconvenience.") == 0)
      {
        raise(SIGINT);
      }
    }
  }
  return NULL;
  pthread_exit(0);
}

void* sendToProcess(void* clientSock)
{
  usleep(2000);
  int clientSocket = *((int*)clientSock);
  char buffer[1024];
  int i = 0;
  while(i == 0)
  {
    bzero(buffer, 1024);
    fflush(stdout);
    printf(":");
    scanf("%[^\n]%*c", buffer);
    if(strlen(buffer) == 0) 
    {
        printf(":");
        scanf("%*[\n]");
    }
    else
    {
        if(inputChecker(buffer) == 0) 
        {
            write(clientSocket, buffer, 700);
            sleep(2);
        }
    } 
  }
  return NULL;
  pthread_exit(0);
}

int inputChecker(char * str) 
{
    if(strlen(str) > 264)
    {
        printf("ERROR! Input string too large.\n");
        return -1;
    }
    
    char cmd[265]; bzero(cmd, 265);
    int i = 0;
    while (i < strlen(str) && str[i] != ' ') 
    {
        cmd[i] = str[i];
        i++;
    }
    if (i > 8 || i < 3) 
    {
        printf("ERROR! Command entered *%s* is INVALID!!!\n", str);
        return -1;
    }
    //Incrementing loc to skip one space after cmd if any	
    i++;
    char arg[256]; bzero(arg, 256);
    int j = 0;
    while (i < strlen(str))
    {
        if (j < 255) 
        {
            arg[j++] = str[i++];
        }
        else 
        {
            printf("ERROR! Argument string is larger than 255 characters!\n");
            return -1;
        }
    }
    return commandHandler(cmd, arg);
}

int commandHandler(char* cmd, char* arg)
{
    if(strcmp(cmd, "create") == 0)
    {
        if(strlen(arg) == 0)
        {
            printf("ERROR! 'create' command requires an account name.\n\n");
            return -1;
        }
    }
    else if(strcmp(cmd, "serve") == 0)
    {
        if(strlen(arg) == 0)
        {
            printf("ERROR! 'serve' command requires the account name.\n\n");
            return -1;
        }
    }
    else if(strcmp(cmd, "deposit") == 0)
    {
        if(strlen(arg) != 0)
        {
            if(isDigit(arg) == 0 || atof(arg) == 0.0)
            {
                printf("ERROR! Invalid Amount entered.\n\n");
                return -1;
            }
        }
        else
        {
            printf("ERROR! 'deposit' command requires the amount.\n\n");
            return -1;
        }
        
    }
    else if(strcmp(cmd, "withdraw") == 0)
    {
        if(strlen(arg) != 0)
        {
            if(isDigit(arg) == 0 || atof(arg) == 0.0)
            {
                printf("ERROR! Invalid Amount entered.\n\n");
                return -1;
            }
        }
        else
        {
            printf("ERROR! 'withdraw' command requires the amount.\n\n");
            return -1;
        }   
    }
    else if(strcmp(cmd, "query") == 0)
    {
        if(strlen(arg) != 0)
        {
            printf("ERROR! 'query' command does not require any argument.\n\n");
            return -1;
        }
    }
    else if(strcmp(cmd, "end") == 0)
    {
        if(strlen(arg) != 0)
        {
            printf("ERROR! 'end' command does not require any argument.\n\n");
            return -1;
        }
    }
    else if(strcmp(cmd, "quit") == 0)
    {
        if(strlen(arg) != 0)
        {
            printf("ERROR! 'quit' command does not require any argument.\n\n");
            return -1;
        }
        raise(SIGINT);
    }
    else
    {
        printf("ERROR! Command entered *%s* is INVALID!!!\n\n", cmd);
        return -1;
    }
    return 0;
}

void termination()
{
    printf("Thank you for using our services. Good Bye!\n");
    write(clientSocket, "quit", 5);
    close(clientSocket);
    exit(0);
}

int isDigit(char* str)
{
    int i = 0, len = strlen(str), isDigit = 1; //isDigit = true.
    char c;
    while(i < (len) && isDigit == 1)
    {
        c = str[i++];
        if((c < '0' || c > '9') && c != '.')//true == not a digit.
        {
            isDigit = 0;
        }
    }
    return isDigit;		
}
