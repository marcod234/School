/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                            Alberto Olvera                           |
|                             Joshua  Tews                            |
|                            CSCE 3600:001                            |
|                            May 4, 2016                              |
|                                                                     |
|            Major Assignment 2 - Sockets & Synchronization           |
|                                                                     |
|   This program accepts connections from clients reading data they   |
|   sent through their sockets and adding it to the total amount.     |
|   Once a client has been accepted it is passed off to a thread      |
|   which handles each client separately. Once both clients have      |
|   quit the server closes all open sockets and exits.                |
|                                                                     |
|   This file needs to be compiled with the -lpthread flag            |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFFER 11 // max number of digits for an int + \0

pthread_mutex_t mutex;

int total = 0;
void *totalUpdate(void *sock2);// function for threads

int main(int argc, char** argv)
{
  int sock, sock2, portnum, j, i = 0;
  struct sockaddr_in local, remote;
  socklen_t client = sizeof(remote);
  pthread_t tid[2];

  if(argc != 2)//check for correct amount of paramters
  {
    fprintf(stderr,"usage: %s <svr_port>\n", argv[0]);
    exit(0);
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);//set up socket
  if(sock == -1)
  {
    perror("Socket");
    exit(0);
  }

  portnum = atoi(argv[1]);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(portnum);

  if(bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0)
  {
    perror("Bind");
    exit(0);
  }

  if(listen(sock, 5)< 0)
  {
    perror("Listen");
    exit(0);
  }

  pthread_mutex_init(&mutex, NULL);//initialize mutex

  while( (sock2 = accept(sock, (struct sockaddr *) &remote, &client)) )
  {
     //pthread was segfaulting when passed (void*) sock2, solution found here:
     int* new_sd = malloc( sizeof( *new_sd )) ; //http://stackoverflow.com/questions/19232957/pthread-create-passing-an-integer-as-the-last-argument
     *new_sd = sock2;

     printf("Client Connection Accepted\n");

     if(pthread_create(&tid[i], NULL, totalUpdate, new_sd) < 0)//creating threads
     {
       perror("Pthread");
       exit(0);
     }

     i++;
     if(i == 2)//allow server to quit
       break;
  }

  for(j = 0; j < 2; j++)
    pthread_join(tid[j],NULL);

  pthread_mutex_destroy(&mutex);
  close(sock);
  return 0;
}

void *totalUpdate(void *sock2)
{
  int wr, data, num, place = 0;
  char buffer[BUFFER];

  int new_sock = *(int *)sock2;

  while(1)//read from client and update total until client quits
  {
    bzero(buffer,BUFFER);
    wr = read(new_sock, buffer, BUFFER);
    buffer[strcspn(buffer, "\n")] = '\0';

    if(wr > 0)
    {
      data = atoi(buffer);

      pthread_mutex_lock(&mutex);//dont allow any other thread to update total

      total+= data;
      num = total;

      while(num!=0)//find the number of digits
      {
        num/=10;
        place++;
      }

      printf("%d\n", total);
      sprintf(buffer,"%d",total);
      buffer[place] = '\0';
      place = 0;
      wr = write(*(int *)sock2, buffer, strlen(buffer)); //write back to client

      pthread_mutex_unlock(&mutex);
    }

    else
    {
      printf("Client Disconnected\n");
      close(new_sock);
      return;
    }
  }
}
