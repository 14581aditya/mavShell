/*
Aditya Mishra
1001663720
axm3720
cse 3320-002
*/



//liscensed by
// The MIT License (MIT)
// 
// Copyright (c) 2016, 2017, 2021 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h> //header file 
#include <unistd.h> // header file 
#include <sys/types.h>//header file 
#include <sys/wait.h> //header file 
#include <stdlib.h> //header file 
#include <errno.h> //header file 
#include <string.h> //header file 
#include <signal.h> //header file 
#include <stdbool.h> //header file 
#include <math.h> //header file 

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line


#define MAX_COMMAND_SIZE 255  // the maximum command lin size

#define MAX_NUM_ARGUMENTS 10  // msh cell only supports 10 arguments

#define MAX_HISTORY_SIZE 15   // as mentioned in the assignment it should be able to 
                              // count the history of 15 last commands

#define MAX_PID_SIZE 15       // as mentioned in the assignment it should be able to
                              // count the history of 15 last commands


int a=0;  // keep track of the number of command lines enter

//int exit= 1;

char *max_hist[MAX_COMMAND_SIZE]; // stores the command entered by the user

int max_pid[MAX_PID_SIZE] ; // array of integers to store PIDs of child process

int hist_count =0;  // keeps track of the command lines entered

int pids_count =0;    // keep track of the number of child process executed

//function for pid counts
void pids(int count)
{
  
  for (int a=0; a < count; a++)
  {
    printf("%d) %d \n", a, max_pid[a]);
  }
}

//function for history count
void history(int count)
{
  
  for (int a=0; a <= count && a<=15; a++)
  {
    printf("%d) %s", a, max_hist[a]);
  }
}

//function for history count
void upper_history(int count)
{
  for(int b=0; b<15; b++)
    {
      max_hist[b]=max_hist[hist_count-14];
      printf("%d. %s",b, max_hist[b]);
      hist_count++;
    }
}

int main()
{
  
  char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);

  while(1)
  {
     // if-statement will give us "msh>"" until we type exit or quit. 
     if(true)
     {
       //prints out the msh prompt
      printf("msh>" );

      //read commands from the command lines
      //maximum number of commands that it can read is ten
      //this while command waits until the user inputs something
      //or else it returns NULL if there is no input
      
      while(!fgets(cmd_str,MAX_COMMAND_SIZE,stdin));
     }

     //parse input
     char *token[MAX_NUM_ARGUMENTS];

     int token_count=0;

     //Pointer to point to the token
     //parsed by strsep
     char *argument_ptr;

     char *working_str=strdup(cmd_str);
     // we are going to move the working_str pointer so
     // keep track of its original value so we can deallocate
     // the correct amount at the end
     char *working_root = working_str;

     int j;
     // Tokenize the input strings with whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    // \TODO Remove this code and replace with your shell functionality
    //int token_index  = 0;
    //for( token_index = 0; token_index < token_count; token_index ++ ) 
    //{
    //printf("token[%d] = %s\n", token_index, token[token_index] );  
    //}

    //shell-assignment 
    //Like from the assignment, it will continue running to infinity and beyond 
    //unless a user input any command or even something wrong.
    max_hist[a]=strdup(cmd_str);

    if(token[0]==NULL)
    {
      continue;
    }
     
     // using "quit" or "exit", to close that command
    else if ((strcmp("quit", token[0]) == 0) || (strcmp("exit", token[0]) == 0))
    {
      _exit(1);//child process should strictly use _exit() to immediately 
               //close the program
    }

    //counting at least the 15 commands processed by the user
    else if(strcmp(token[0], "history")==0)
    {
       if(hist_count>14)
       {
         
          upper_history(hist_count);

       }
       else
       {

          history(hist_count);

       }

    }

      // changes and handles directories cd:
      else if(strcmp(token[0], "cd")==0)
      {
        
        //if the user inputted directory is not present
        if(chdir(token[1]) == -1)
        {
          printf("%s: No such directory. Try again \n", token[1]);
        }
      }
       
      //printing the value of created child process using showpids
      else if (strcmp(token[0], "showpids")==0)
      {
         pids(pids_count);
      }
      

      // in case of !n : the nth command present is executed
      // runs the nth command entered by the user where n is from 1 to 15
      else if(token[0][0]=='!')
      {
        //converted string to integer with the use of atoi().
        int num = atoi(&cmd_str[1]);
        //if the nth command doesnot exist
        if( num> a || num > 15|| num ==0)
        {
          printf("%d:\tthere is no such command.\t\n", num);

        }
        //if the nth command exist
        else
        {

          strcpy(cmd_str, max_hist[num-1]);

          continue;
        }

      }
      
      else
      {
        //using fork(), to create a new child process from the parent
       pid_t pid;

       pid = fork();
      

        if (pid == -1)
        {
          perror("fork has failed to process \n");

          exit(EXIT_FAILURE);
        }
         // when compiling fork(), if it returns 0, we are in the child process.
        else if (pid==0)
        {
          fflush(NULL);
          
          //using execvp to search for an executable file and then replaces it with another file.
          // it runs like/usr/local/bin
        
          int ret= execvp(token[0], token);
          //user inputted invalid arguments
          if(ret==-1)
          {

            printf("%s:\tcommand not found\t\n", *token);
          }

          exit(1);

        }

        else
        {

          fflush(NULL);//using flush to ouput the buffer of the stream.

          int status;

          if(pids_count>14) //using if-statement for  more than 15 child process
          {
            //using for loop to keep track of the child
            for(int b=0; b<15; b++)
            {
              max_pid[b]=max_pid[b+1];
            }
                pids_count--;

          }

          max_pid[pids_count]=pid;

          pids_count++;
          //exiting child
          waitpid(pid, &status, 0);

        }
        
      }
      
      //counting commands

      hist_count = a;

      // increasing commands  
      hist_count++ ;
      a++ ;
      free(working_root) ;



    }
    return 0 ;

}
