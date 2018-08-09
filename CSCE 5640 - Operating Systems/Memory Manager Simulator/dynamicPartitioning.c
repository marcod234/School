#include <stdio.h>
#include <stdlib.h>
#include "processes.h"

char* myMalloc(size_t size, int id);
void coalesce(memNode *walk); 
void myFree(proc *p);
int dynamicPartitioning(proc **set, int cycle);
void printList();

proc **set;
char *block;
memNode *head, *tail;
size_t memAvail = BLOCKSIZE;
  
int main(int argc, char **argv)
{

  int i, cycle;
  FILE *in, *out;
  int dynDone = 0;
  double dynTotal = 0;
  memNode *walk, *walk2, *temp;
  struct timeval dynStart, dynEnd;

  
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
  
  out = fopen("dynOut", "w"); //open file for status outputs
  if(out == NULL)
  {
    printf("could not create file\n");
    exit(EXIT_FAILURE);	
  }
  
  block = (char*)malloc(BLOCKSIZE); //allocate initial 128 MB block 
  
  for(i = 0; i < NUMPROC; i++) //set up list
  {
    walk = (memNode *)malloc(sizeof(memNode));
    walk->id = -1;
    walk->offset = -1;
    walk->size = 0;
    walk->next = NULL;
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
    if(!dynDone) // processes are not finished yet.
    {	
      gettimeofday(&dynStart, NULL);
      dynDone = dynamicPartitioning(set, cycle);
      gettimeofday(&dynEnd, NULL);
	  
      //add this cycle's runtime to total, time in microseconds 
      dynTotal += ((dynEnd.tv_sec * 1000000) + dynEnd.tv_usec) - ((dynStart.tv_sec * 1000000) + dynStart.tv_usec);
    }
	
    if(cycle%CYCPRINT == 0) //print state of processes every CYCPRINT cycles
    { 
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
        fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
    }
	
    if(dynDone)
    {
      dynTotal /= 1000000; //time in seconds	
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
	    fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
	
      fprintf(out, "Dynamic Partitioning Time: %lf seconds\n", dynTotal);
      break;
    }	  
  }
  
  fclose(out);
  
  walk = head;
  for(i = 0; i < NUMPROC; i++)
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
  for(i = 0; i < NUMPROC; i++)
  {
    printf("%d\t%d\t%u\n", walk->id, walk->offset, walk->size);
    walk = walk->next;
  }	
}

char* myMalloc(size_t size, int id)
{
  size_t prevOffset, currOffset;
  memNode *walk, *temp;
  
  if(memAvail < size)
  {
    
    return NULL;
  }

  if(head->id == -1)   
  {
	  head->id = id;
    head->mem = block; //allocate beginning of block
    head->offset = 0;
    head->size = size;
    memAvail -= size;
	  return block;
  }
  else
    for(walk = head->next; walk != NULL; walk = walk->next)
    {
      if(walk->id == -1) //free block
      {
		    prevOffset = walk->prev->offset + walk->prev->size;
        walk->id = id;
        walk->mem = block + prevOffset;
        walk->offset = prevOffset;
        walk->size = size;
        memAvail -= size;
        return block + prevOffset;		
      }
      
	    else //taken block, try to find space bewteen blocks, first fit
      {
        prevOffset = walk->prev->offset + walk->prev->size;
        currOffset = walk->offset;

        if(currOffset -  prevOffset >= size) //found a space between memory
        {
          //unlink block from the end	
          temp = tail;
          tail = tail->prev;
          tail->next = NULL;
          //point block to blocks it needs to go between
          temp->next = walk;
          temp->prev = walk->prev;
          //point outer blocks to inserted block;
          temp->prev->next = temp;
          temp->next->prev = temp;
		  
          temp->id = id;
          temp->mem = block + prevOffset;
          temp->offset = prevOffset;
          temp->size = size;
          return block + prevOffset;
        }
      }
    }
  return NULL; //no space available
}

void coalesce(memNode *walk)
{
  for(walk; walk != NULL; walk = walk->next)
  {
    if(walk->next->id == -1)
      return;

    else //shift all memory down
    {
      walk->next->mem = block + walk->offset + walk->size;
      walk->next->offset = walk->offset + walk->size;
      set[walk->next->id]->memory = walk->next->mem; //update process	  
    }
  }
}

void myFree(proc *p)
{
  size_t offset;
  memNode *walk, *temp;
  
  for(walk = head; walk != NULL; walk = walk->next)
  {
	  if(walk->id == p->id)
    {
      if(walk != head)
      {
        memAvail += walk->size;
        //set default values
        walk->id = -1;
        walk->mem = NULL;  
        walk->offset = -1;
        walk->size = 0;
        //point to block that will move to tail
        temp = walk;
        walk = walk->prev;
        //link outer blocks together
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        //unlink freed block and place at tail
        tail->next = temp;
        temp->prev = tail;
        temp->next = NULL;
        tail = temp;
      }
    
      coalesce(walk);
    
      if(walk == head && walk->id == p->id)
      {
        memAvail += walk->size;
      //set default values
        walk->id = -1;
        walk->mem = NULL;  
        walk->offset = -1;
        walk->size = 0;
        //point to block that needs to be added to the tail
        temp = walk;
        //set next block as head
        walk = walk->next;
        walk->prev = NULL;
        head = walk;
        //unlink block and add to tail
        tail->next = temp;
        temp->next = NULL;
        temp->prev = tail;
        tail = temp;  
      }
    }
  }
  return;
}

int dynamicPartitioning(proc **set, int cycle)
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
  
  for(walk = head; walk->id != -1; walk = walk->next)
  {	
    int id = walk->id;
    if(set[id]->cycles > 0 && set[id]->start != cycle)  //decrement all running processes.
      set[id]->cycles--; 

    if(set[id]->cycles == 0) //free memory if process finishes
    {	 
  
      if(walk == head)
        walk = walk->next;
      else
		    walk = walk->prev; //current walk will be removed, set to prev 
      
      myFree(set[id]);
    
      numFinished++;
      set[id]->cycles = -1;
      set[id]->end = cycle;
    }
  }
  
  if(numFinished == NUMPROC)
    return 1;

  return 0;
}