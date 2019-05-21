//SERVER
//Authors: Manohar Chitoda & Suraj Upadhyay
//Asst: Asst3
//Date: December 12, 2018
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>
#define INTERVAL 15


////////////////////////////////////////////////
//FROM BANKINGFUNCTIONS.C HELPER CLASS
typedef struct bank 
{
    char accountName[255];
    double balance;
    int inService;
    struct bank *next;
}BANK;

//GLOBAL DATA
sem_t csem;
sem_t ssem;
BANK *head = NULL;

//FUNCTION PROTOYTES
void printData();
void create(char*, BANK *, int);
void serve(char*, BANK**, int);
void withdraw(double, BANK*, int);
void deposit(double, BANK*, int);
void query(BANK*, int);
void end(BANK**, int);
void quit(BANK**, int);
BANK* accountFinder(char*);
int isDigit(char*);


////////////////////////////////////////////////
//FROM CLIENTSERVER.C CLASS
typedef struct _ALL_CLIENTS
{
    int * clientFD;
    struct _ALL_CLIENTS * next;
}ALL_CLIENTS;

typedef struct _acptStruct
{
  int serverSokFd;
  struct sockaddr_in sockAddress;
}acptStruct;

//GLOBAL DATA
ALL_CLIENTS *clientHead = NULL;

//FUNCTION PROTOYTES
int createServer(int);
void* startAccept(void*);
void* startClient(void*);
void inputBreaker(char*, int, BANK**);
int commandHandler(char* , char*, int, BANK**);
void termination();

int main(int argc, char *argv[]) 
{
  if(argc == 2)
  {
    //char * str = "26196";
    char * str = argv[1];
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
    
    sem_init(&csem, 0, 1);
    sem_init(&ssem, 0, 1);

    //Setting up itimer struct values
    struct itimerval tout_val;
    tout_val.it_interval.tv_sec = INTERVAL;
    tout_val.it_interval.tv_usec = 0;
    tout_val.it_value.tv_sec = INTERVAL;
    tout_val.it_value.tv_usec = 0;

    //starting the timer
    setitimer(ITIMER_REAL, &tout_val, 0);
    signal(SIGALRM, printData); 
    signal(SIGINT, termination);

    createServer(port);
    sem_destroy(&csem);
    sem_destroy(&ssem);
    return 0;
  }
  else
  {
      fprintf(stderr, "Invalid arguments!\n");
      return -1;
  }
}

//Create a server
int createServer(int port)
{
  int serverSokFd = socket(AF_INET, SOCK_STREAM, 0);
  if(serverSokFd < 0)
  {
    printf("Socket not created.\n");
    return -1;
  }
  struct linger so_linger;
  so_linger.l_onoff = 1;
  so_linger.l_linger = 0;
  setsockopt(serverSokFd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

  struct sockaddr_in sockAddress;
  sockAddress.sin_family = AF_INET;
  sockAddress.sin_port = htons(port);
  char ip[255]; bzero(ip, 255);
  gethostname(ip, 255);

  struct hostent *hp = gethostbyname(ip);
  printf("My ip name is: %s\n", ip);
  bcopy((char*)hp->h_addr, (char*)&sockAddress.sin_addr, hp->h_length);
  
  if(bind(serverSokFd, (struct sockaddr *)&sockAddress, sizeof(sockAddress)) < 0)
  {
    printf("Binding Error.\n");
    printf("Error code: %d\n", errno);
    printf("%s\n",strerror(errno));
    return -1;
  }
  if(listen(serverSokFd, 500) < 0)
  {
    printf("Listening Error.\n");
  }
  
  acptStruct* acpt = malloc(sizeof(acptStruct));
  acpt->serverSokFd = serverSokFd;
  acpt->sockAddress = sockAddress;
  
  pthread_t t = NULL;
  pthread_create(&t, NULL, startAccept, acpt);
  pthread_join(t, NULL);
  
  close(serverSokFd);	
  return 0;
}

// Start acceting clients
void* startAccept(void* struc)
{
  acptStruct * acpt = (acptStruct*) struc;
  int i = 1;
  while(i == 1)
  {
    ALL_CLIENTS * clientNode = (ALL_CLIENTS*)malloc(sizeof(ALL_CLIENTS));
    
    int len = sizeof(acpt->sockAddress);
    int* newClient = (int*)(malloc(sizeof(int)));
    *newClient = accept(acpt->serverSokFd, (struct sockaddr *)&(acpt->sockAddress), (socklen_t *)&len);
    if(*newClient < 0)
    {
      fprintf(stderr, "Accepting Error.\n");
      return NULL;
    }
    clientNode->clientFD = newClient;
    clientNode->next = clientHead;
    clientHead = clientNode;
    printf("New client %d connected.\n",*newClient);
    pthread_t t = NULL;
    pthread_create(&t, NULL, startClient, newClient);
  }
  return NULL;
}

//Start executing client messages
void* startClient(void* clientFd)
{
  int client = *((int*)clientFd);
  //printf("New Client Started\n");
  char * display = "create <accountname (char) > \nserve <accountname (char) > \ndeposit <amount (double) > \nwithdraw <amount (double) > \nquery \nend \nquit";
  write(client, display, strlen(display));
  
  BANK *serving = NULL;
  
  int i = 0;
  while(i == 0)
  {
    char buffer[1024]; bzero(buffer, 1024);
    int dataRead = read(client, buffer, 700);
    
    if(dataRead > 0)
    {      
      inputBreaker(buffer, client, &serving);
    }
  }
  return NULL;
}

//Breaks down the input from client message
void inputBreaker(char * str, int client, BANK ** serving) 
{
    if(strlen(str) > 264)
    {
        write(client, "ERROR! Input string too large.\n", 40);
        return;
    }
    char cmd[265]; bzero(cmd, 265);
    char msg[1024]; bzero(msg, 1024);

    int i = 0;
    //store in temp
    while (i < strlen(str) && str[i] != ' ') 
    {
        cmd[i] = str[i];
        i++;
    }
    if (i > 8 || i < 3) 
    {
        bzero(msg, 1024);
        sprintf(msg, "ERROR! Command entered *%s* is INVALID!!!\n", str);
        write(client, msg, strlen(msg));
        return;
    }
    //Incrementing loc to skip the space between cmd and arg if any	
    i++;
    char arg[256];
    bzero(arg, 256);

    int j = 0;
    while (i < strlen(str))
    {
        if (j < 255) 
        {
            arg[j++] = str[i++];
        }
        else 
        {
            bzero(msg, 1024);
            sprintf(msg, "Argument string larger than 255 characters!\n");
            write(client, msg, strlen(msg));
            return;
        }
    }
    commandHandler(cmd, arg, client, serving);
    return;
}

//Handles what command to execute
int commandHandler(char* cmd, char* arg, int client, BANK ** serving)
{
    char msg[1024]; bzero(msg, 1024);
    if(strcmp(cmd, "create") == 0)
    {
        if(strlen(arg) != 0)
        {
            sem_wait(&csem);
            create(arg, *serving, client);
            sem_post(&csem);
        }
        else
        {
            sprintf(msg, "ERROR! 'create' command requires the account name.\n");
            write(client, msg, strlen(msg));
        }
        
    }
    else if(strcmp(cmd, "serve") == 0)
    {
        if(strlen(arg) != 0)
        {
            sem_wait(&ssem);
            serve(arg, serving, client);
            sem_post(&ssem);
        }
        else
        {
            sprintf(msg, "ERROR! 'serve' command requires the account name.\n");
            write(client, msg, strlen(msg));
        }
        
    }
    else if(strcmp(cmd, "deposit") == 0)
    {
        if(strlen(arg) != 0)
        {
            double amt = atof(arg);
            if(amt != 0.0)
            {
                deposit(amt, *serving, client);
            }
            else
            {
                sprintf(msg, "ERROR! Invalid Amount entered.\n");
                write(client, msg, strlen(msg));
            }
        }
        else
        {
            sprintf(msg, "ERROR! 'deposit' command requires the amount.\n");
            write(client, msg, strlen(msg));
        }
        
    }
    else if(strcmp(cmd, "withdraw") == 0)
    {
        if(strlen(arg) != 0)
        {
            double amt = atof(arg);
            if(amt != 0.0)
            {
                withdraw(amt, *serving, client);
            }
            else
            {
                sprintf(msg, "ERROR! Invalid Amount entered.\n");
                write(client, msg, strlen(msg));
            }
        }
        else
        {
            sprintf(msg, "ERROR! 'withdraw' command requires the amount.\n");
            write(client, msg, strlen(msg));
        }
        
    }
    else if(strcmp(cmd, "query") == 0)
    {
        query(*serving, client);
    }
    else if(strcmp(cmd, "end") == 0)
    {
        end(serving, client);
    }
    else if(strcmp(cmd, "quit") == 0)
    {
        quit(serving, client);
    }
    else
    {
        sprintf(msg, "ERROR! Command entered *%s* is INVALID!!!\n", cmd);
        write(client, msg, strlen(msg));
    }
    return 0;
}

// Creates a new account ONLY IF NOT SERVING ANY ACCOUNT.
void create(char * acctName, BANK *serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    if(serving == NULL)
    {
        if (accountFinder(acctName) == NULL) 
        {
            if (strlen(acctName) > 255) 
            {
                sprintf(msg, "ERROR! Account name larger than 255 characters.\n");
            }
            else
            {
                // Create an account of BANK type
                BANK *acct = (BANK*) malloc(sizeof (BANK));

                //Store the account name
                snprintf(acct->accountName, 255, "%s", acctName);

                //Intial balance is 0
                acct->balance = 0.0;

                //Point new acct to the old first node
                acct->next = head;

                // Point head to the first node
                head = acct;
                sprintf(msg, "Account *%s* created!\n", acctName);
            }
        }
        else
        {
            sprintf(msg, "ERROR! Account *%s* already exist.\n", acctName);
        }
    }
    else
    {
        sprintf(msg, "ERROR!  New Account cannot be created while serving an account.\n");
    }
    write(client, msg, strlen(msg));
    return;
}

// Serves a specific account ONLY IF NOT SERVING ANY ACCOUNT.
void serve(char * name, BANK** serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    if(*serving == NULL)
    {
        BANK * acct = accountFinder(name);
        if (acct != NULL) 
        {
            if(acct->inService == 0)
            {
                acct->inService = 1;
                *serving = acct;
                sprintf(msg, "Account *%s* is being served.\n", name);
            }
            else
            {
                sprintf(msg, "ERROR! Account *%s* is already being served currently by other client", name);
            }          
        } 
        else 
        {
            sprintf(msg, "ERROR! The account *%s* does not exist.\n", name);
        }
    }
    else
    {
        //TEST FOR %s--> account name being served.
        sprintf(msg, "ERROR! Can only serve one account at one time.\n");
    }
    write(client, msg, strlen(msg));
    return;
}

//Deposit amount to current serving account
void deposit(double amount, BANK *serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    if(serving != NULL)
    {
        if (amount >= 0) 
        {
            serving->balance += amount;
            sprintf(msg, "Current balance: $%lf\n", serving->balance);
        } 
        else 
        {
            sprintf(msg, "ERROR! Invalid Amount entered.\n");            
        }
    }
    else 
    {
        sprintf(msg, "ERROR! No Account is currently being served.\n");
    }
    write(client, msg, strlen(msg));
    return;
}

//Withdraws amount from current serving account
void withdraw(double amount, BANK *serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);    
    if (amount > 0) 
    {
        if (serving != NULL) 
        {
            if (serving->balance >= amount) 
            {
                serving->balance -= amount;
                sprintf(msg, "Current balance: $%lf\n", serving->balance);
            }
            else 
            {
                sprintf(msg, "ERROR! Entered amount exceeds available balance.\n");
            }
        }
        else 
        {
            sprintf(msg, "ERROR! No Account is currently being served.\n");
        }
    }
    else 
    {
        sprintf(msg, "ERROR! Invalid Amount entered.\n");
    }
    write(client, msg, strlen(msg));
    return;
}

// Returns the current account balance.
void query(BANK *serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    if (serving != NULL) 
    {
        sprintf(msg, "\nAccount Name: *%s*\nAccount Balance: $%lf\n", serving->accountName, serving->balance);
    }
    else 
    {
        sprintf(msg, "ERROR! No Account is currently being served.\n");
    }
    write(client, msg, strlen(msg));    
    return;
}

//Ends current running session
void end(BANK ** serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    if (*serving != NULL) 
    {
        sprintf(msg, "Ending current serving session.\n");
        (**serving).inService = 0;
        *serving = NULL;
    }
    else 
    {
        sprintf(msg, "ERROR! No Account is currently being served.\n");
    }
    write(client, msg, strlen(msg));
    return;
}

//Terminates the process
void quit(BANK ** serving, int client) 
{
    char msg[1024]; bzero(msg, 1024);
    printf("Client %d is disconnected.\n",client);
    sprintf(msg, "Thank You for using our services. Good Bye!");
    write(client, msg, strlen(msg));
    if(*serving != NULL)
    {
        (**serving).inService = 0;
        *serving = NULL;
    }
    pthread_exit(0);
}

BANK* accountFinder(char *acctName) 
{
    if (head != NULL) 
    {
        BANK *current = head;
        
        //while the bank account is not found, keep searching
        while (current != NULL) 
        {
            if ((strcmp(acctName, current->accountName) == 0)) 
            {
                return current;
            }
            current = current->next;
        }
    } 
    return NULL;
}

void printData() 
{
    printf("***BANK DATA REPORT***\n");
    BANK *curr = (BANK*) malloc(sizeof(BANK));
    if (head != NULL) 
    {
        curr = head;
        while(curr != NULL) 
        {
            char * service = "Account in service.";
            if(curr->inService == 0)
            {
                service = "Account NOT in service.";
            }
            printf("%s\t$%lf\t%s\n", curr->accountName, curr->balance, service);
            curr = curr->next;
        }
    } 
    else 
    {
        printf("No accounts in the bank.\n");
    }
    printf("***END OF REPORT***\n\n");
}

void termination()
{
    printf("\nBank server is shutting down.\n");
    printf("Locking all accounts.\n");
    ALL_CLIENTS * curr = clientHead;
    while(curr != NULL)
    {
        write(*(curr->clientFD), "Bank server is shutting down. Sorry for the inconvenience.", 100);
        curr = curr->next;
    }
    //sleep(1);
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

