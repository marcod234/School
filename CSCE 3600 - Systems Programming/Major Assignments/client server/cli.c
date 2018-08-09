#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER 11 // max number of digits for an int + \0

pid_t setConnection(char* local_port);
void clientConnect(char* ipAddr, char* cli_port, int data, char clinum);
float cpuUtil();

int main(int argc, char** argv)
{
  int sock, svr_port, cli1_port, cli2_port, wr, data, num, total = 0, place = 0;
  struct sockaddr_in server;
  char clinum, buffer[BUFFER];
  socklen_t servlen = sizeof(server);
  pid_t pid;
  float cpu, threshold = atoi(argv[5]);

  if(argc != 6)
  {
    fprintf(stderr,"usage: %s <rem_ipaddr> <svr_port> <cli1_port> <cli2_port> <cpu_%%>\n", argv[0]);
    exit(0);
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    perror("Socket");
    exit(0);
  }

  clinum = argv[0][8];
  svr_port = atoi(argv[2]);
  server.sin_family = AF_INET;
  server.sin_port = htons(svr_port);

  if(clinum == '1')
  {
    pid = setConnection(argv[3]);
    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 0) // connect to local host
    {
      perror("Inet_pton");
      exit(0);
    }
  }

  else
  {
    pid = setConnection(argv[4]);
    if(inet_pton(AF_INET, argv[1], &server.sin_addr) < 0)
    {
      perror("Inet_pton");
      exit(0);
    }

  }

  if(pid > 0)
  {
    if(connect(sock, (struct sockaddr *) &server, servlen) < 0 )
    {
      perror("Connect");
      exit(0);
    }

    while(1)
    {
      bzero(buffer, BUFFER);
      printf("Enter CLIENT %c Data: ", clinum);
      scanf("%d", &data);
      cpu = cpuUtil();

      if(cpu > threshold)
      {
        int send = total - data;

        kill(pid,SIGKILL);

        if(total == 0)
          send = data;

        if(clinum == '1')
          clientConnect(argv[1], argv[4], send, clinum);
        else if(clinum == '2')
          clientConnect(argv[1], argv[3], send, clinum);

        break;
      }

      else if(data == 0)
      {
        printf("CLIENT %c Disconnecting...\n", clinum);
        sprintf(buffer,"DISCONNECT");
        buffer[10] = '\0';
        wr = write(sock, buffer,strlen(buffer));

        kill(pid,SIGKILL);
        break;
      }

      num = data;

      while(num!=0)//find the number of digits
      {
        num/=10;
        place++;
      }

      sprintf(buffer,"%d",data);
      buffer[place] = '\0';
      wr = write(sock, buffer,strlen(buffer));

      wr = read(sock,buffer,BUFFER);

      if(wr > 0)
      {
        buffer[strcspn(buffer, "\n")] = '\0';
        total = atoi(buffer);
        printf("SERVER Total: %d\n", total);
      }
    }
    close(sock);

    return 0;
  }
}

pid_t setConnection(char* local_port)
{
  pid_t pid = fork();

  if(pid > 0) //parent
    return pid;

  else if(pid == 0) //child proccess
  {
    int portnum, sock, sock2, wr;
    struct sockaddr_in local, remote;
    socklen_t client = sizeof(remote);
    char buffer[BUFFER];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
      perror("Socket");
      exit(0);
    }

    portnum = atoi(local_port);
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(portnum);

    if(bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0)
    {
      perror("Bind");
      exit(0);
    }

    if(listen(sock,5) < 0)
    {
      perror("Listen");
      exit(0);
    }

    sock2 = accept(sock, (struct sockaddr *) &remote, &client);
    bzero(buffer, BUFFER);
    wr = read(sock2, buffer, BUFFER);
    buffer[strcspn(buffer,"\n")] = '\0';

    printf("Recieved %s from Other Client\n", buffer);
    close(sock2);
    close(sock);
  }


  //else
    //setConnection(local_port);//try to create child again if fails
}

void clientConnect(char* ipAddr, char* cli_port, int data, char clinum)
{
  int sock, cliPort, num, wr, place = 0;
  char buffer[BUFFER];
  struct sockaddr_in server;
  socklen_t servlen = sizeof(server);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    perror("Socket");
    exit(0);
  }

  cliPort = atoi(cli_port);
  server.sin_family = AF_INET;
  server.sin_port = htons(cliPort);

  if(inet_pton(AF_INET, ipAddr, &server.sin_addr) < 0)
  {
    perror("Inet_pton");
    exit(0);
  }

  if(connect(sock, (struct sockaddr *) &server, servlen) < 0 )
  {
    perror("Connect");
    exit(0);
  }

  num = data;
  while(num!=0)//find the number of digits
  {
    num/=10;
    place++;
  }

  bzero(buffer, BUFFER);
  sprintf(buffer,"%d",data);
  buffer[place] = '\0';

  if(clinum == '1')
    clinum++;
  else
    clinum--;

  printf("Sending %d to CLIENT %c\n", data, clinum);
  wr = write(sock, buffer, strlen(buffer));
  close(sock);
}

float cpuUtil()
{
    char read[50];
    float cpu = 0, num[5];
    FILE *fp;

    sprintf(read, "top -b -n1 | grep 'Cpu(s)' > cpu");
    system(read);
    fp = fopen("cpu", "r");
    fscanf(fp, "%*s %f %*s %f %*s %f %*s %*f %*s %*f %*s %f %*s %f", &num[0], &num[1], &num[2], &num[3], &num[4]);

    cpu = num[0] + num[1] + num[2] + num[3] + num[4];
    printf("CPU is %f\n", cpu);

    return cpu;
}
