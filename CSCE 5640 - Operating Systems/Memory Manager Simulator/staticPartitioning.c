#include <stdio.h>
#include <stdlib.h>
#include "processes.h"

char* myMalloc(size_t size, int id);
void myFree(proc *p);
int staticPartitioning(proc **set, int cycle);
void printList();

proc **set;
char *block;
memNode *head, *tail;

int main(int argc, char **argv)
{
  int i, cycle;
  FILE *in, *out;
  int stDone = 0;
  int numBlocks;
  double stTotal = 0;
  memNode *walk, *walk2, *temp;
  struct timeval stStart, stEnd;
  
  if(argc != 2)
  {
    printf("usage: %s <filename>\n", argv[0]);
    exit(EXIT_FAILURE);	
  }
  
  set = (proc **) malloc(NUMPROC * sizeof(proc *));
  for(i = 0; i < NUMPROC; i++)
    set[i] = (proc *) malloc(sizeof(proc));

  in = fopen(argv[1], "r"); //open file to read in processes
  if(in == NULL)
  {
    printf("could not open file %s\n", argv[1]);
    exit(EXIT_FAILURE);	
  }
  
  for(i = 0; i < NUMPROC; i++) //store all processes
  {
    fscanf(in, "%d %u %d", &set[i]->id, &set[i]->memSize, &set[i]->cycles);
    set[i]->start = set[i]->end = -1;
  }

  fclose(in);
  
  out = fopen("stOut", "w"); //open file for status outputs
  if(out == NULL)
  {
    printf("could not create file\n");
    exit(EXIT_FAILURE);	
  }
  
  block = (char*)malloc(BLOCKSIZE); //allocate initial 128 MB block 
  
  numBlocks = BLOCKSIZE/PARTITION;
  
  for(i = 0; i < numBlocks; i++) //set up list
  {
    walk = (memNode *)malloc(sizeof(memNode));
    walk->id = -1;
    walk->offset = i * PARTITION;
    walk->size = 0;
    walk->next = NULL;
    walk->mem = block + walk->offset;
    if(i == 0)
    {
      head = walk;
      head->prev = NULL;
      tail = head;
    }
    else
    {
      walk->prev = tail;
      tail->next = walk;
      tail = tail->next;
    }
    walk = walk->next;
  }

  for(cycle = 0; 1; cycle++) //starting simulation
  { 
    if(!stDone) // processes are not finished yet.
    {	
      gettimeofday(&stStart, NULL);
      stDone = staticPartitioning(set, cycle);
      gettimeofday(&stEnd, NULL);
	  
      //add this cycle's runtime to total, time in microseconds 
      stTotal += ((stEnd.tv_sec * 1000000) + stEnd.tv_usec) - ((stStart.tv_sec * 1000000) + stStart.tv_usec);
  	}

    if(cycle%CYCPRINT == 0) //print state of processes every CYCPRINT cycles
    { 
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
        fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
    }
	
    if(stDone)
    {
      stTotal /= 1000000; //time in seconds	
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
        fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
	
      fprintf(out, "Static Partitioning Time: %lf seconds\n", stTotal);
        break;
    }	  
  }
  
  fclose(out);
  
  walk = head;
  for(i = 0; i < BLOCKSIZE/PARTITION; i++)
  {
    temp = walk;
    walk = walk->next;
    free(temp);	
    free(set[i]);
  }
  free(set); 
 
  free(block);
  
  return 0;	
}

void printList()
{
	int i;
	memNode *walk, *walk2;
  walk = head;
  walk2 = tail;
  
  printf("ID\toffset\tsize\n");
  for(i = 0; i < BLOCKSIZE/PARTITION; i++)
  {
    printf("%d\t%d\t%u\n", walk->id, walk->offset, walk->size);
    walk = walk->next;
  }	
}

char* myMalloc(size_t size, int id)
{
  memNode *walk, *temp;

  if(head->id == -1)   
  {
	  head->id = id;
    head->size = size;
	  return head->mem;
  }
  else
    for(walk = head->next; walk != NULL; walk = walk->next)
      if(walk->id == -1) //free block
      {
        walk->id = id;
        walk->size = size;
        return walk->mem;		
      }
  return NULL; //no space available
}

void myFree(proc *p)
{
  size_t offset;
  memNode *walk, *temp;
  
  for(walk = head; walk != NULL; walk = walk->next)
  {
	  if(walk->id == p->id)
    {
      //set default values
      walk->id = -1;
      p->memory = NULL;      
      walk->size = 0;
      return;
    }
  }
  return;
}

int staticPartitioning(proc **set, int cycle)
{
  int i;
  proc *p;
  memNode *walk;
  static int nextID = 0;
  static int numFinished = 0;
  
  if(cycle%50 == 0 && nextID != NUMPROC) //enter a new process every 50 cycles
  {
    set[nextID]->memory = myMalloc(set[nextID]->memSize, set[nextID]->id); //allocate needed memory
    if(set[nextID]->memory != NULL)
    {
      set[nextID]->start = cycle;
      nextID++;
    }
  }
  
  for(walk = head; walk != NULL; walk = walk->next)
  {	
    int id = walk->id;
    if(id != -1)
    {
      if(set[id]->cycles > 0 && set[id]->start != cycle)  //decrement all running processes.
        set[id]->cycles--; 

      if(set[id]->cycles == 0) //free memory if process finishes
      {	       
        myFree(set[id]);
    
        numFinished++;
        set[id]->cycles = -1;
        set[id]->end = cycle;
      }
    }
  }
  
  if(numFinished == NUMPROC)
    return 1;

  return 0;
}