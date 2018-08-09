/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                            Alberto Olvera                           |
|                             Joshua  Tews                            |
|                            CSCE 3600:001                            |
|                           March  19, 2016                           |
|                                                                     |
|	          Major Assignment 1 - The Shell and System Calls           |
|                                                                     |
|   This program aims to implement a simple shell that aims to run    |
|   and parse commands similarly to the way UNIX/LINUX does.          | 
|                                                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE 512

void interactive();
void batch(char* batchFile);
int cmdProcess(char* line);
void cmdRun(char* ch, char* test);
void newPrompt(char* prompt);
void fre_sh_a_vac_a_do();

char* prompt;

int main(int argc, char** argv)
{// cd and exit should be finished now
 // 
  if (argc == 1)
    interactive();
  else if (argc == 2)
    batch(argv[1]);
  else 
    fprintf(stderr, "usage: shell [batchfile]");

  return 0;
}

void interactive()
{
  char* line = calloc(SIZE, sizeof(char));
	prompt = calloc(SIZE, sizeof(char));
	strcpy(prompt, "cmds>");
  int quit;
  printf("---------------- Interactive Mode ----------------\n");
  printf("type 'prompt' at any point to change the prompt\n");
  while(1)
  {
		printf("%s ", prompt);
    fgets(line, SIZE, stdin);
		line[strcspn(line, "\n")] = '\0'; // remove trailing newline

		if(feof(stdin))//^D
    {
	    printf("\n^D was recieved\n");
	    return;//exit shell 
    }
    else
		{
      if( line[0] != '\n' )
        quit = cmdProcess(line);
    }
		
   while(wait(NULL) > 0){}

		if(quit){
			free(line);
			exit(0);
		}
  }
}

void batch(char* batchFile)
{
  char* line = calloc(SIZE, sizeof(char));
  int quit;
	printf("------------------- Batch Mode -------------------\n");
  FILE* batch;
  batch = fopen(batchFile, "r");
  
	/*checking if the file opened correctly*/
	if(batch == NULL){
    fprintf(stderr, "Error: file \"%s\" does not exist", batchFile);
		free(line);
    exit(1);
  }
	
	/*actual file parsing*/
	while(fgets(line, SIZE, batch) != NULL){
		printf("%s\n", line);
	  quit = cmdProcess(line);
		
		while(wait(NULL) > 0){}
		
	  if(quit){
	    fclose(batch);
		  free(line);
		  exit(0);
	  }
	}

	return;
}

char* fixLine(char* line)
{
	char* buffer = calloc(strlen(line),sizeof(char));
  int i=0, j=0; /*iterating variables*/
	int start = 1; /*boolean variable for tracking start of commands*/
	
	for(i = 0; line[i] != '\0'; i++)/*loop through each character*/
	{
		if(line[i] != ' '){/*once we hit a character set start to 0*/
			start = 0;
		}
		if(line[i] == ';'){/* if we are at the end of a command check for ending spaces*/
			while(j > 0 && buffer[j-1] == ' ')
				j--;
			buffer[j] = line[i];
			j++;
			start = 1;
		}
		else if(start == 0){/*if we passed the starting spaces start taking charcters*/
			buffer[j] = line[i];
			j++;
		} 
	}	
	buffer[j] = '\0';/* end the buffer*/
	for(i=0; i<=j;i++)
		line[i]=buffer[i];
	free(buffer);
	return line; 
}

int cmdProcess(char *line)
{
  char* ch;//whole comand substring
	char* test, *test2;//temporary string for testing
	int quit = 0;

  line = fixLine(line);
  ch = strtok(line, ";");
	
  while(ch != NULL)
  {
	  /*get command without flags or paths*/
		test = calloc(strlen(ch)+19, sizeof(char));//allocate for whole command plus "which > /dev/null"
		test2 = calloc(strlen(ch), sizeof(char));
		test2 = strncpy(test2,ch,strcspn(ch," "));//gets base command;

		if(strcmp(test2,"cd")==0 && strcmp(ch,"cd")!=0)//check for change directory
			chdir(ch+3);
		
		else if(strcmp(ch, "prompt") == 0)
		  newPrompt(prompt);
		
		else if(strcmp(ch, "fre_sh_a_vac_a_do") == 0)
	    fre_sh_a_vac_a_do();
		
		else if(strcmp(test2,"exit")==0 || strcmp(test2,"quit") == 0)//prepare for exiting program
		    quit = 1;
	
		else
		{
			/*modify test for a system check*/
			strcpy(test,"which ");
			strcat(test,test2); //append the base command
			strcat(test," > /dev/null" ); //append dev/null/ redirection
			
			
			if(system(test) == 0) /*test if command is valid*/
 				cmdRun(ch, test);/*run the commands*/
			else//command does not exist
			  fprintf(stderr, "Error: command %s does not exist", ch);
		}
		
		ch = strtok(NULL,";");
	  free(test);
	  free(test2);
	}
  return quit;
}

void cmdRun(char* ch, char* test)
{ 
	pid_t a;
    if( !(a = fork()) ) //in child
    {
     system(ch);
		 printf("\n");
     exit(0);
    }
	  else if(a == -1)
		  fprintf(stderr, "Error: no child process\n");
}

void newPrompt(char* prompt)
{
	printf("Enter your new prompt: ");
	fgets(prompt, SIZE, stdin);
	prompt[strcspn(prompt, "\n")] = '>'; //replace newline char from fgets with >
	prompt[strcspn(prompt, ">")+1] = '\0'; //add null character after >
}

void fre_sh_a_vac_a_do() //Josh REALLY wanted to add this
{
	printf("fresh\n");
	printf("https://www.youtube.com/watch?v=tw8vK4TuQXM\n");
	return;
}
