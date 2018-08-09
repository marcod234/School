#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "processes.h"

int main(int argc, char **argv)
{
  int i;
  FILE *f;
  proc *p; //pointer to a process
  pid_t pid[3];
  float total = 0;
  struct timeval time;
  char *progs[3] = {"system", "dynamic", "static"};  //programs each child process will run
  
  f = fopen("proc", "w"); //will output set of processes to file. 
  
  if(f == NULL)
  {
    printf("Failed to open file\n");
    exit(EXIT_FAILURE);	
  }
  
  gettimeofday(&time);
  srandom(time.tv_sec ^ time.tv_usec); //seeding the prng
  
  for(i = 0; i < NUMPROC; i++) //generate processes with random cycles and memory requirements.
  {
    p = procGen();
    fprintf(f, "%d\t%d\t%d\n", p->id, p->memSize, p->cycles);
    total += p->memSize;
    free(p);
  }
  printf("total size of processes memory: %.0f B / %f MB\n", total, (float)(total/1048576));

  fclose(f); //flush to file
  
  for(i = 0; i < 3; i++) //create 3 processes to run each type of partitioning
  {
    if((pid[i] = fork()) == 0)
    {
      execl(progs[i], progs[i], "proc", (char*) NULL);
      waitpid(pid[i], NULL, NULL);
    }
	
    else if(pid[i] == -1)
    {
      printf("child process not created");
      exit(EXIT_FAILURE);	  
    }

  }
  
  printf("All processes have finished. View files: sysOut, dynOut, stOut\n");

  return 0;	
}