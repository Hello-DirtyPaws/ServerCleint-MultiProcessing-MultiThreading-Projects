#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "simpleCSVsorter.h"





RECORD* divide(RECORD *temp, int size)
{
   
    if(size > 1)
    {   
        RECORD *temp1 = temp;
        int i;
        for(i = 1; i < size/2; i++)
        {
            temp1 = temp1->next;
        }
        RECORD *x = temp1->next;
        temp1->next = NULL;
        temp1 = x;
        
        temp = divide(temp, size/2);
                
        temp1 = divide(temp1, size - size/2);
                
        RECORD * sorted = NULL;
            sorted = merge(temp, temp1, sorted);
        
        
        return sorted;
    }
    return temp;
    
}

int getSize(RECORD *curr)
{
    int i = 0;
    
    while(curr != NULL)
    {
        i++;
        curr = curr->next;
    }
    
    return i;
}

RECORD* merge(RECORD *n1, RECORD *n2, RECORD *sorted)
{
    
    RECORD *temp;
    if (n1 == NULL) 
    {
        while (n2 != NULL) 
        {
            temp = n2->next;
            sorted = addLast(sorted, n2);
            n2 = temp;
        }
        return sorted;
    } 
    if (n2 == NULL) 
    {
        while (n1 != NULL) 
        {
            temp = n1->next;
            sorted = addLast(sorted, n1);
            n1 = temp;
        }
        return sorted;
    }
    else //n1 != null and n2 != null
    {
        if(compareTo(n1->array[key],n2->array[key]) <= 0)
        {
            temp = n1->next;
            sorted = addLast(sorted, n1);
            return merge(temp, n2, sorted);
        } 
        else
        {
            temp = n2->next;
            sorted = addLast(sorted, n2);
            return merge(n1, temp, sorted);
        }
    }
    return sorted;
}

RECORD* addLast(RECORD *temp, RECORD *n)
{    
    if (temp == NULL) 
    {
        n->next = NULL;
        return n;
    } 
    else 
    {
        RECORD *curr = temp;
        while (curr->next != NULL) 
        {
            curr = curr->next;
        }
        curr->next = n;
        n->next = NULL;
    }
    return temp;    
}

///////////////////////////////////////////////////////


///STRING MANIPULATORS
///////////////////////////////////////////

char* trim(char* str)
{
    char temp[strlen(str)];
    char* finalStr = NULL;
    int i = 0, j = 0, len = strlen(str);
    
    
    if(len == 0)
    {
        return "";
    }
    
    else
    {
        char c = str[0];
        while(i < len && (c == ' ' || c == '\"'))
        {
            c = str[++i];
        }
        
        int trailSpace = 0;
        while(i < len && c != '\"')
        {
            if(c == ' ')
            {
                trailSpace++;
            }
            else
            {
                trailSpace = 0;
            }
            temp[j++] = c;
            c = str[++i];
        }
        
        /* if including quotes, use (j - trailSpace - 1) in the loop
         * and after the loop add the last char using
         * finalStr[i] = temp[j - 1];
         * j is the length of the temp string.
         */ 
        finalStr = (char*)malloc(j * sizeof(char));
        for(i = 0; i < (j - trailSpace); i++)
        {
            finalStr[i] = temp[i];
        }
                
       /* This is weird..there is a randon char added in the end
        * so have to manually add any arbitary char and replace
        * it by NULL char.
        */
        finalStr[i] = 'F';
        finalStr[i] = '\0';
       
    }
    //free(temp);
    return finalStr;
}

int isDigit(char* str)
{
    int i = 0, len = strlen(str), isDigit = 1; //isDigit = true.
    char c;
        
    while(i < (len-1) && isDigit == 1)
    {
        c = str[i++];
        if((c < '0' || c > '9') && c != '.')//true == not a digit.
        {
            isDigit = 0;
        }
    }
            
    return isDigit;
        
}

int compareTo(char* str1, char* str2)
{
    if(str1 == NULL)
    {
        return -1;
    }
    if(str2 == NULL)
    {
        return 1;
    }
    char* str1Cpy = (str1);
    char* str2Cpy = (str2);
    
  
    int flag1 = isDigit(str1Cpy);
    int flag2 = isDigit(str2Cpy);
    
    if(flag1 != flag2)
    {
        //One of the str is not an int value
        //thus performing the strcmp
        return strcmp(str1Cpy, str2Cpy);
    }
    else if(flag1 == 0)
    {
        return (strcmp(str1Cpy, str2Cpy));
    }
    else
    {
        return(atof(str1Cpy)-atof(str2Cpy));
    }
}

