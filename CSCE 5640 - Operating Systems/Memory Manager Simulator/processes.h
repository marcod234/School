#ifndef PROCESS_H
#define PROCESS_H

#define BLOCKSIZE   104857600 //100 MB
#define PARTITION   5242880   //5MB
#define MAXSIZE     2097152   //2MB
#define MINSIZE     10240     //10KB
#define MAXCYC      2500
#define MINCYC      200
#define NUMPROC     500
#define CYCPRINT    1000      //number of cycles bewteen each status print

typedef struct process{
  int id;
  size_t memSize; //how much space is needed
  int cycles;     //how long it will run
  int start;      //which cycle the process started in
  int end;        //which cycle the process ended in
  char* memory;
} proc;

typedef struct memNode{
  int id;        //id of the process holding the memory;
  char* mem;     //pointer to allocated memory
  int offset;    //offest of the memory from the base address of the memory block
  size_t size;
  struct memNode *next; //used also as right child in binary tree
  struct memNode *prev; //used also as left child in binary tree
} memNode;

proc* procGen()
{
  static int id = 0;
  
  proc *temp = (proc*)malloc(sizeof(proc));
  
  temp->id = id; //set process id
  temp->memSize = (size_t)(random()%(MAXSIZE - MINSIZE + 1) + MINSIZE); //10KB <-> 2MB
  temp->cycles = random()%(MAXCYC - MINCYC + 1) + MINCYC; // 200 <-> 2500 cycles
  
  id++; //increment id 
  return temp;
}

#endif