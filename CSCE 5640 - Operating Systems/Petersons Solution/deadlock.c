//Marco Duarte
//marcoduarte2@my.unt.edu

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork, read, write
#include <fcntl.h>  //open
#include <sys/stat.h> //file permissions

int main(int argc, char** argv)
{
  int fd, num;
  char buf[4]; //range will go from 0 to 100
  pid_t pid;
  
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
  
  else if(pid == 0) // child process
  {
    while(1)
    {
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
		break;
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
      }    
	}
  }
  
  else if(pid >= 0) //parent process
  {
    while(1)
    {
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
		break;
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
      }    
	}
  }
 
  return 0;	
}