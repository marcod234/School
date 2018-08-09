#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(void)
{
  sem_t *write, *read;  
  write = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  read = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  FILE *f, *f2;
  pid_t p, p2;
  
  sem_init(write, 1, 1); //2nd arg lets semaphore be shared between processes, 3rd arg is initial value.
  sem_init(read, 1, 1);
  
  p = fork();
  if(p == 0) //1st child process
  {
    char buff[30];
	while(1)
    {
	  sem_wait(write);      //wait to write to writeFile
	  printf("%d : writing to writeFile\n", getpid());
      f = fopen("writeFile", "w");
	  fprintf(f, ":)\n");
	  fclose(f);
	  sem_wait(read);       //wait to read from readFile
	  f2 = fopen("readFile", "r");
	  printf("%d : reading from readFile\n", getpid());
	  fscanf(f2, "%s", buff);
	  sem_post(read);
	  sem_post(write);   
    }
	
  }
  else if(p > 0)
  {
    p2 = fork();
    if(p2 == 0) //2nd child process
    {
      char buff[30];
	  while(1)
      {
	    sem_wait(read);      //wait to write to readFile
	    printf("%d : writing to readFile\n", getpid());
        f = fopen("readFile", "w");
	    fprintf(f, ";)\n");
	    fclose(f);
	    sem_wait(write);       //wait to read from writeFile
	    printf("%d : reading from writeFile\n", getpid());
	    f2 = fopen("writeFile", "r");
	    fscanf(f2, "%s", buff);
	    sem_post(write);
	    sem_post(read);   
      }
    }
	else if(p > 0) //continue if parent
    {
      
    }
    else
    {
      printf("Could not create child 2\n");
	  exit(EXIT_FAILURE);
    }
  }
  else
  {
	printf("Could not create child 1\n");
	exit(EXIT_FAILURE);
  }
  
  //wait for children to finish
  waitpid(p, NULL, 0);
  waitpid(p2, NULL, 0);
  
  //destroy semaphores
  sem_destroy(write);
  sem_destroy(read);
  
  //unmap from shared memory
  munmap(write, sizeof(sem_t));
  munmap(read, sizeof(sem_t));
  return 0;	
}