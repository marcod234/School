DESCRIPTION:
  This program will solve the three list problem with semaphores and output the operations 
  each process does to files p1 (process 1 output), p2 (process 2 output), and p3 (process 3 output) 

COMPILE:
  gcc lists.c -lpthread
  
USAGE:
  ./a.out <listSize>
  ./a.out <listSize> <executionTime>
  
  If you run the program with the first option the program will run until Ctrl-C is given as input to stop the program.
  
  The second option allows you to specify a time in milliseconds at which, after that time has passed, the parent process will kill processes 1,2, and 3.
  
OUTPUT:
  Writes to files p1, p2, p3. 