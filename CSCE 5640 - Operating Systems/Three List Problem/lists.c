//Marco Duarte
//marcoduarte2@my.unt.edu

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork, usleep
#include <semaphore.h>
#include <sys/stat.h> //file permissions
#include <sys/mman.h> //mmap
#include <signal.h> //SIGTERM

#define  getName(x) #x //turns variable into string name

void printList(int *l, int length, FILE *p);
void initList(int *l, int length, int currSize);

int unLink(int* l, char* name, FILE *p);
void Link(int n, int* l, char* name, FILE *p);
void produce_information_in_block(int* b);
void use_block_x_to_produce_info_in_y(int* x, int* y);
void consume_information_in_block(int *c);

void process1(int* freelist, int* list_1);
void process2(int* freelist, int* list_1, int* list_2);
void process3(int* freelist, int* list_2);

sem_t *FlCount, *L1Count, *L2Count, *useFL, *useL1, *useL2, *FlCount2; 
FILE *p1, *p2, *p3;
int maxLen;

int main(int argc, char** argv)
{
  int *freelist, *list_1, *list_2;
  void *fl, *L1, *L2;
  int listRegionSize;
  pid_t pid, pid2, pid3;
  size_t sem = sizeof(sem_t);
  unsigned int procRun;

  
  srand(time(NULL));
  
  if(argc < 2 || argc > 3)
  {
	printf("usage: %s [list length > 1] [number of microseconds to let processes run, leave blank for infinite]\n", argv[0]);
    exit(EXIT_FAILURE);	
  }
  
  if(argc == 3)
	procRun = atoi(argv[2]);
	
  maxLen = atoi(argv[1]);
  if(maxLen < 2)
  {
	printf("usage: %s [list length > 1]\n", argv[0]);
    exit(EXIT_FAILURE);	
  }
  
  listRegionSize = (sizeof(int)*(maxLen+1)); //need to make space for the list and each node.
  
  //place each list into shared memory. 
  fl = mmap(NULL, listRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  if(fl == MAP_FAILED)
  {
	printf("Could not map freelist to shared memory\n");
	exit(EXIT_FAILURE);
  }
  
  L1 = mmap(NULL, listRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  if(L1 == MAP_FAILED)
  {
	printf("Could not map list_1 to shared memory\n");
	exit(EXIT_FAILURE);
  }
  
  L2 = mmap(NULL, listRegionSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  if(L2 == MAP_FAILED)
  {
	printf("Could not map list_12 to shared memory\n");
	exit(EXIT_FAILURE);
  }
  
  //place each semaphore into shared memory. 
  FlCount = mmap(NULL, sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  L1Count = mmap(NULL, sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  L2Count = mmap(NULL, sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  useFL = mmap(NULL,  sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  useL1 = mmap(NULL,  sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  useL2 = mmap(NULL,  sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  FlCount2 = mmap(NULL, sem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  
  freelist = (int*)(fl); //assign mapped memory to lists
  initList(freelist, maxLen, maxLen); //do some initialization of lists
  
  list_1 = (int*)(L1);
  initList(list_1, maxLen, 0);
  
  list_2 = (int*)(L2);
  initList(list_2, maxLen, 0);

  //initialize semaphores
  sem_init(FlCount, 1, maxLen-1); //2nd arg lets semaphore be shared between processes, 3rd arg is initial value.
  sem_init(L1Count, 1, 0); //nothing on list_1 and list_2 initially 
  sem_init(L2Count, 1, 0);
  sem_init(useFL, 1, 1); //binary semaphores to control access to lists. 
  sem_init(useL1, 1, 1);
  sem_init(useL2, 1, 1);
  sem_init(FlCount2, 1, maxLen);
  
  p1=fopen("p1","w");
  p2=fopen("p2","w");
  p3=fopen("p3","w");
  
  //start forking processes 
  pid = fork();
  
  if(pid < 0) 
  {
	printf("Child process not created\n");
	exit(EXIT_FAILURE); 
  }
  
  if(pid == 0) //child
    process3(freelist, list_2);
    
  
  if(pid > 0) //Parent
   {
	pid2 = fork();
	
	if(pid2 < 0)
	{
	  printf("Child process not created\n");
	  exit(EXIT_FAILURE); 
    }
	
	if(pid2 == 0)
	  process2(freelist, list_1, list_2);
	
	if(pid2 > 0)
	{
	  pid3 = fork();

      if(pid3 < 0)
	  {
	    printf("Child processes not created\n");
		exit(EXIT_FAILURE);
	  }

	  if(pid3 == 0)
		process1(freelist, list_1);
	}
	
	if(pid3 > 0)
	{
	  if(procRun == 0) //run infinitely
	    while(1);
      else
	  {
	    usleep(procRun); //kill processes after a user defined time.
		kill(pid, SIGTERM);
		kill(pid2, SIGTERM);
		kill(pid3, SIGTERM);
	  }
	}
  }

  //unmap from shared memory
  munmap(fl, listRegionSize);
  munmap(L1, listRegionSize);
  munmap(L2, listRegionSize);
  munmap(FlCount, sem);
  munmap(L1Count, sem);
  munmap(L2Count, sem);
  munmap(useFL, sem);
  munmap(useL1, sem);
  munmap(useL2, sem);
  munmap(FlCount2, sem);

  //destroy semaphores
  sem_destroy(FlCount);
  sem_destroy(L1Count);
  sem_destroy(L2Count);
  sem_destroy(useFL);
  sem_destroy(useL1);
  sem_destroy(useL2);
  sem_destroy(FlCount2); 
  return 0;	
}

void printList(int *l, int length, FILE *p)
{
  int i;

  fprintf(p, "length: %d\nlist:\t ", l[maxLen]);
  for(i = 0; i < l[maxLen]; i++)
  {
	fprintf(p, "%d\t", l[i]);
  }	
  fprintf(p, "\n");
  fflush(p);
}

void initList(int* l, int length, int currSize)
{
  int i;
  
  for(i = 0; i < currSize; i++)
    l[i] = -1;

  l[length] = currSize;
}

int unLink(int* l, char* name, FILE *p)
{
  int i;
  int temp = l[0]; //get block from front of list
  int currSize = l[maxLen];
  
  fprintf(p, "-----------\n%s Before unlink:\n", name);
  printList(l, currSize, p);
  
  for(i = 0; i < currSize; i++)
  {
	if(i == currSize-1)
      l[i] = -1;	
    else  
	  l[i] = l[i+1];
  }
   
  l[maxLen] = --currSize;
  
  fprintf(p,"%s after unlink:\n", name);
  printList(l, currSize, p);
  fprintf(p, "-----------\n");
  return temp;
}

void Link(int n, int* l, char* name, FILE *p)
{
  int currSize = l[maxLen];
  fprintf(p, "-----------\n%s Before link:\n", name);
  printList(l, currSize, p);
	
  l[currSize] = n;
  l[maxLen] = ++currSize;
	
  fprintf(p, "%s after link:\n", name);
  printList(l, currSize, p);
  fprintf(p, "-----------\n");
}

void produce_information_in_block(int* b)
{
	
	*b = rand()%100 + 1; //generate random number for info.
}

void use_block_x_to_produce_info_in_y(int* x, int* y)
{
	*y = (*x + 5640)%600; //produce some value.
    *x = -1; //set to -1 since it will be put back on the freelist. 	
}

void consume_information_in_block(int *c)
{
	*c = -1;
}

void process1(int* freelist, int* list_1)
{
  int b, i;
  while(1)
  {
    sem_wait(FlCount); //wait if there is only one block available.
    sem_wait(FlCount2);//wait if there are no blocks available   
    sem_wait(useFL); //wait until no other process is using freelist
	
    b = unLink(freelist, getName(freelist), p1);
    sem_post(useFL); //wake up any waiting processes. 
	
    produce_information_in_block(&b);
	
    sem_wait(useL1);
    Link(b, list_1, getName(list_1), p1);
    sem_post(useL1);
    sem_post(L1Count);//wakes up any process waiting for a block to be added to list_1	

  }
}

void process2(int* freelist, int* list_1, int* list_2)
{
  int x, y, i;
  while(1)
  {
	sem_wait(L1Count); //wait to remove if no blocks
	sem_wait(useL1); //wait for turn
    x = unLink(list_1, getName(list_1), p2);
	sem_post(useL1);
	
	sem_wait(FlCount2);
	sem_wait(useFL);
    y = unLink(freelist, getName(freelist), p2);
	sem_post(useFL);
	sem_post(FlCount);
	
    use_block_x_to_produce_info_in_y(&x, &y);
	
	sem_wait(useFL);
    Link(x, freelist, getName(freelist), p2);
	sem_post(useFL);
	sem_post(FlCount2);
	
	sem_wait(useL2);
    Link(y, list_2, getName(list_2), p2);
	sem_post(useL2);
	sem_post(L2Count);
  }
}

void process3(int* freelist, int* list_2)
{
  int c, i;
  while(1)
  {
	sem_wait(L2Count);
	sem_wait(useL2);
    c = unLink(list_2, getName(list_2), p3);
	sem_post(useL2);
	
	consume_information_in_block(&c);
	
	sem_wait(useFL);
	Link(c, freelist, getName(freelist), p3);
	sem_post(useFL);
	sem_post(FlCount2);
  }
}