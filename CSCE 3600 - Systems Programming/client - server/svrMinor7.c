/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 3600:001                            |
|                           April  11, 2016                           |
|                                                                     |
|               Minor Assignment 7 - Using Linux Sockets              |
|                                                                     |
|    This is the server portion of the program which fulfills         |
|    requests from two separate clients at the same time. Clients     |
|    can have the option between buying from SIZE amount of tickets   |
|    and also returning them. The server continously runs until both  |
|    clients have disconnected.                                       |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 20 // # of tickets
#define BUFFER 256

int bufferTest(int sock, char* buffer, int clinum, int* tickets, int* sold, int num);//all requests will be processed here

int main(int argc, char** argv)
{
  int sock, sock2, sock3, i, portnum, numTix = SIZE, j, clinum = 0;
  socklen_t client;
  struct sockaddr_in local, remote;
  int tickets[SIZE], sold[SIZE];
  char buffer[BUFFER];
  if(argc != 2)//check for correct amount of paramters
  {
    fprintf(stderr,"usage: %s [port]\n", argv[0]);
    exit(1);
  }

  srand(time(NULL)); //set up random number generator

  for(i = 0; i < SIZE; i++) //generate tickects
  {
    tickets[i] = rand() % 90000 + 10000;
    sold[i] = -1;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);//create socket
  if(sock == -1)
  {
    perror("Socket");
    exit(1);
  }

  portnum = atoi(argv[1]);//set socket info
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(portnum);
  if(bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0)
  {
    perror("Bind");
    exit(1);
  }

  if(listen(sock, 5) < 0)//listen for 1st client
  {
    perror("listen");
    exit(1);
  }


  sock2 = accept(sock, (struct sockaddr *) &remote, &client);

  if(sock2 == -1)
  {
    perror("Accept");
    exit(1);
  }

  if(listen(sock, 5) < 0)//listen for 2nd client
  {
    perror("2nd listen");
    exit(1);
  }

  sock3 = accept(sock, (struct sockaddr *) &remote, &client);


  while(1)//service both clients
  {
    clinum += bufferTest(sock2, buffer, clinum, tickets, sold, 1);
    clinum += bufferTest(sock3, buffer, clinum, tickets, sold, 2);

    if(clinum == 2)//number of disconnected clients
      break;
  }

  close(sock3);
  close(sock2);
  close(sock);

  return 0;
}

int bufferTest(int sock, char* buffer, int clinum, int* tickets, int* sold, int num)
{

    int wr, i, tix, sum = 0;

    bzero(buffer,BUFFER);
    wr = read(sock, buffer, BUFFER); //read from socket
    buffer[strcspn(buffer, "\n")] = '\0';

    if(wr < 0)
      fprintf(stderr, "ERROR: reading from socket\n");

    if(strcmp(buffer,"DISCONNECTED") == 0) //client has finished
      return 1;//updates count of disconnected clients in main


    if(clinum < 2)//at least one client still running
    {
      if(strcmp(buffer, "BUY") == 0)// buy request
      {
        printf("[CLIENT %d]: BUY\n", num);

        for(i = 0; i < SIZE; i++)
        {
           sum+= tickets[i];
        }
        if(sum > 0)
        {
          i = 0;
          while(i!= SIZE && tickets[i] == 0)//look for a non purchased ticket
            i++;

          sprintf(buffer,"%d",tickets[i]); //put ticket on buffer to send to client
          buffer[5] = '\0';

          sold[i] = tickets[i];//add sold ticket to "sold" array
          tickets[i] = 0;//no longer available for purchase
          printf("[SERVER X]: Client %d buy %d\n", num, sold[i]);
        }

        else
        {
          sprintf(buffer, "Database Full");
          printf("[SERVER X]: Database Full\n");
        }
      }

      else if(strstr(buffer, "RETURN") != NULL) //return request
      {
        printf("[CLIENT %d]: %s\n", num, buffer);
        tix = atoi(buffer+7); //get ticket portion of buffer
        for(i = 0; i < SIZE; i++)
          if(sold[i] == tix) //check if ticket is valid
          {
            sold[i] = -1; //set sold to default valuee
            tickets[i] = tix;//make ticket  available for purchase again
            printf("[SERVER x]: Client %d cancel %d\n", num, tix);
            break;
          }
        if(i == SIZE)
        {
          printf("[SERVER x]: Client %d cancel %d failed\n", num, tix);// ticket not valid
          sprintf(buffer, "Ticket not exist");
          buffer[16] = '\0';
        }
      }

      wr = write(sock, buffer, strlen(buffer));//send message back
      if(wr < 0)
        fprintf(stderr, "ERROR: writing to socket\n");

    }
    return 0; //no disconnect
}

