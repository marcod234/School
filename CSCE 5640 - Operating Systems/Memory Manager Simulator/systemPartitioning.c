#include <stdio.h>
#include <stdlib.h>
#include "processes.h"

int systemPartitioning(proc **set, int cycle);

int main(int argc, char** argv)
{
  proc **set;
  int i, cycle;
  FILE *in, *out;
  int sysDone = 0;
  double sysTotal = 0;
  struct timeval sysStart, sysEnd;
  
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
  
  out = fopen("sysOut", "w"); //open file for status outputs
  if(out == NULL)
  {
    printf("could not create file\n");
    exit(EXIT_FAILURE);	
  }
  
  for(cycle = 0; 1; cycle++) //starting simulation
  { 
    if(!sysDone) // processes are not finished yet.
    {	
      gettimeofday(&sysStart, NULL);
      sysDone = systemPartitioning(set, cycle);
      gettimeofday(&sysEnd, NULL);
	  
      //add this cycle's runtime to total, time in microseconds 
      sysTotal += ((sysEnd.tv_sec * 1000000) + sysEnd.tv_usec) - ((sysStart.tv_sec * 1000000) + sysStart.tv_usec);
    }
	
    if(cycle%CYCPRINT == 0) //print state of processes every CYCPRINT cycles
    { 
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
	    fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
    }
	
    if(sysDone)
    {
      sysTotal /= 1000000; //time in seconds	
      fprintf(out, "\ncycle %d\nID\tmemSize\t\tcycles\tstart\tend\n", cycle); //print final cycle
      for(i = 0; i < NUMPROC; i++)
	    fprintf(out, "%d\t%d\t\t%d\t%d\t%d\n", set[i]->id, set[i]->memSize, set[i]->cycles, set[i]->start, set[i]->end);
	
      fprintf(out, "System Partitioning Time: %lf seconds\n", sysTotal);
      break;
    }	  
  }
  
  fclose(out);
  for(i = 0; i < NUMPROC; i++)
	free(set[i]);
  free(set); 
  
  return 0;
}

int systemPartitioning(proc **set, int cycle)
{
  int i;	
  static int nextID = 0;
  static int numFinished = 0;
   
  if(cycle%50 == 0 && nextID != NUMPROC) //enter a new process every 50 cycles
  {
    set[nextID]->memory = malloc(set[nextID]->memSize); //allocate needed memory
    set[nextID]->start = cycle;	
    nextID++;
  }
  
  for(i = 0; i < nextID; i++)
  {	
    if(set[i]->cycles > 0 && set[i]->start != cycle)  //decrement all running processes.
      set[i]->cycles--; 

    if(set[i]->cycles == 0) //free memory if process finishes
    {	
      free(set[i]->memory);
      numFinished++;
      set[i]->cycles = -1;
      set[i]->end = cycle;
    }
  }  
  
  if(numFinished == NUMPROC) //return true when all processes have finished.
    return 1;

  return 0;
}