/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 3600:001                            |
|                           March  22, 2016                           |
|                                                                     |
|          Minor Assignment 6 - Implementing Command Line Pipe        |
|                                                                     |
|    This program aims to recreate what the UNIX/LINUX system does    |
|    with piped commands on the command line using child processes    |
|    which write and read to/from a pipe using file descriptors to    |
|    chain the output of the first user specifed command to the       |
|    second command.                                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum {READ, WRITE};

int main( int argc, char** argv )
{
  int fd[2];
  pid_t a,b;

  if( argc == 3 ) //exactly two commands to be piped
  {

    if(pipe(fd) == -1)
    {
      perror("Pipe");
      exit(1);
    }

    printf("executing: %s | %s\n", argv[1], argv[2]);

    if(!( a = fork() ))
    {
      char** test = malloc(sizeof(char*)); //copy first command from argv
      if(test == NULL)
      {
	 perror("Bad malloc");
	 exit(5);
      }

      if((test[0] = malloc(sizeof(argv[1]))) == NULL)
      {
         perror("Bad malloc");
         exit(5);
      }
      test[0] = argv[1];

      dup2(fd[WRITE], fileno(stdout)); //write to pipe
      close(fd[READ]); //close original file descriptor
      close(fd[WRITE]);
      execvp(argv[1], &test[0]);
      perror("Child 1");
      exit(3);
    }

    if(!(b = fork()))
    {
      dup2(fd[READ], fileno(stdin));//read from pipe
      close(fd[READ]);
      close(fd[WRITE]);
      execvp(argv[2], &argv[2]);
      perror("Child 2");
      exit(4);
    }

      close(fd[READ]);
      close(fd[WRITE]);

  }

  else
    printf("error: too few or too many arguments: %d\n", argc);

    wait(0);//waiting for both processes to finish
    wait(0);

  printf("command line pipe completed\n");
  return 0;
}

