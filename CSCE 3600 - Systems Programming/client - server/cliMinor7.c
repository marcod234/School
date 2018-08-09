/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 3600:001                            |
|                           April  11, 2016                           |
|                                                                     |
|               Minor Assignment 7 - Using Linux Sockets              |
|                                                                     |
|    This is the client portion of the program which makes requests   |
|    to buy or return a ticket from the server continuously until     |
|    the specified amount of REQUESTS are made.                       |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define REQUESTS 15
#define BUFFER 256

void buy(int* sock, char* buffer, int* tickets);
void retrn(int* sock, char* buffer, int* tickets);

int main(int argc, char** argv)
{
  int sock, portnum, num, i, wr;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  socklen_t servlen = sizeof(serv_addr);
  char buffer[BUFFER] = {0};
  int tickets[REQUESTS];

  for(i =0; i < REQUESTS; i++)
     tickets[i] = 0;

  if(argc != 3)//check for corect number of parameters
  {
    fprintf(stderr,"usage: %s [hostname] [port]\n", argv[0]);
    exit(1);
  }

  portnum = atoi(argv[2]);//get the port
  sock = socket(AF_INET, SOCK_STREAM, 0);//create socket
  if (sock == -1)
  {
    perror("Socket");
    exit(1);
  }

  server = gethostbyname(argv[1]);
  if(server == NULL)
  {
    fprintf(stderr, "ERROR: no such host\n");
    exit(0);
  }

  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
    (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);

  serv_addr.sin_port = htons(portnum);
  if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)//attmpt to connect to server
  {
    perror("connect");
    exit(0);
  }

  for(i = 0; i < REQUESTS; i++)//make requests
  {
    int chance = rand() % 10 + 1; // 80/20 chance of getting buy/return
    if(chance > 2)
      buy(&sock, buffer, tickets);
    else
      retrn(&sock, buffer, tickets);
  }

  bzero(buffer, BUFFER);
  sprintf(buffer, "DISCONNECTED"); //let server know that client has finished
  buffer[12] = '\0';
  write(sock,buffer,strlen(buffer));
  close(sock);

  printf("[CLIENT]: Database Table:\n");//print out purchased tickets
  for(i = 0; i < REQUESTS; i++)
    printf("[%d]: %d\n", i, tickets[i]);

  return 0;
}

void buy(int* sock, char* buffer, int* tickets)
{
  int wr, i;

  bzero(buffer, BUFFER);
  strcpy(buffer, "BUY");
  buffer[3] = '\0';

  wr = write(*sock,buffer,strlen(buffer));//send request to buy
  if(wr == -1)
   fprintf(stderr,"ERROR: writing to socket\n");

  wr = read(*sock,buffer,BUFFER);

  if(wr == -1)
   fprintf(stderr,"ERROR: reading from socket\n");

  if(strstr(buffer,"Database") == NULL)
  {
    buffer[5] = '\0';

    for(i = 0; i < REQUESTS; i++) //find an empty space to place number
    {
      if(tickets[i] == 0)
      {
        tickets[i] = atoi(buffer);//convert string from buffer to int
        break;
      }
    }
  }


  printf("[CLIENT]: BUY\n");

  printf("[SERVER]: %s\n", buffer);

}

void retrn(int* sock, char* buffer, int* tickets)
{
  srand(time(NULL));
  int wr, i, index = rand() %REQUESTS;


  bzero(buffer, BUFFER);
  sprintf(buffer, "RETURN %d", tickets[index]);
  buffer[12] = '\0';
  printf("[CLIENT]: RETURN %d\n", tickets[index]);

  wr = write(*sock,buffer,strlen(buffer));//inform server of ticket return request
  if(wr == -1)
   fprintf(stderr,"ERROR: writing to socket\n");

  wr = read(*sock,buffer,BUFFER);
  if(wr == -1)
   fprintf(stderr,"ERROR: reading from socket\n");

  printf("[SERVER]: %s\n", buffer);
  if(strstr(buffer,"RETURN") != NULL)//return succesfull
  {
    printf("[CLIENT]: Ticket %d returned\n", tickets[index]);
    tickets[index] = 0;
  }
  else
    printf("[CLIENT]: Return failed\n");

}

