//Marco Duarte
//marcoduarte2@my.unt.edu

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork, read, write
#include <fcntl.h>  //open
#include <sys/stat.h> //file permissions
#include <sys/mman.h>
int main(int argc, char** argv)
{
  int fd, num;
  char buf[4]; //range will go from 0 to 100
  pid_t pid;
  
  //shared memory example from
  //http://binaryprose.com/2014/03/shared-variables-between-child-processes-in-c-fork/
  int* c1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
				 
  int* c2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
				 
  int* will_wait = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
						
  *c1 = 0;
  *c2 = 0;

  if(argc != 2)
  {
    printf("usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  pid = fork();
  
  if(pid < 0)
  {
	printf("Child process not created\n");
	exit(EXIT_FAILURE);
  }
  
  else if(pid == 0) // child process, c1
  {
    while(1)
    {
	  *c1 = 1;
	  *will_wait = 1;
	  
	  while(*c2 && (*will_wait == 1)); //process will loop here
	  
	  //critical section begin
	  //open file
      if( (fd = open(argv[1], O_RDONLY)) == -1) //check if file exists and open for reading
      {
	    printf("File \"%s\" does not exist\n", argv[1]);
        exit(EXIT_FAILURE);	
      }
	  
	  //read number from file
	  read(fd, buf, 3); //read 3 bytes at a time
	  
	  close(fd);
	  
	  //convert string to int
	  num = atoi(buf);
	  
	  printf("PID = %d\tN = %d\n", getpid(), num);
	  
	  if(num == 100)
	  {
		*c1 = 0;
		break;
	  }
	  else
	  {
	    num++;
	    sprintf(buf, "%d", num);
	  
	    if( (fd = open(argv[1], O_WRONLY)) == -1) //check if file exists and open for reading
        {
	      printf("File \"%s\" does not exist\n", argv[1]);
          exit(EXIT_FAILURE);	
        }
	  
	    write(fd, buf, 3);
	    //flush file http://man7.org/linux/man-pages/man2/fsync.2.html
	    fsync(fd);
	    close(fd);
		*c1 = 0;
		//critical section end
      }    
	}
  }
  
  else if(pid >= 0) //parent process, c2
  {
    while(1)
    {
      *c2 = 1;
	  *will_wait = 2;
	  
	  while(*c1 && (*will_wait == 2)); //process will loop here
	  
	  //critical section begin
	  //open file
      if( (fd = open(argv[1], O_RDONLY)) == -1) //check if file exists and open for reading
      {
	    printf("File \"%s\" does not exist\n", argv[1]);
        exit(EXIT_FAILURE);	
      }
	  
	  //read number from file
	  read(fd, buf, 3); //read 3 bytes at a time
	  
	  close(fd);
	  
	  //convert string to int
	  num = atoi(buf);
	  
	  printf("PID = %d\tN = %d\n", getpid(), num);
	  
	  if(num == 100)
	  {
		*c2 = 0;
		break;
	  }
	  else
	  {
	    num++;
	    sprintf(buf, "%d", num);
	  
	    if( (fd = open(argv[1], O_WRONLY)) == -1) //check if file exists and open for reading
        {
	      printf("File \"%s\" does not exist\n", argv[1]);
          exit(EXIT_FAILURE);	
        }
	  
	    write(fd, buf, 3);
	    //flush file http://man7.org/linux/man-pages/man2/fsync.2.html
	    fsync(fd);
	    close(fd);
		*c2 = 0;
		//critcal section end
      }    
	}
  }
 
  return 0;	
}