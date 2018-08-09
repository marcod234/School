/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 3600:001                            |
|                           January 30, 2016                          |
|                                                                     |
|    This program parses a few commands that the user enters,         |
|    specified by the flags -i, -g, -s, and -o,from the command       |
|    line and then runs it using the "system()" function              |
|                                                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_builder(int i, int j, char* command, char** argv); //function prototype

int main(int argc, char **argv) // argc and argv used to parse commands 
{
  char command[80] = "cat < ";  // string that will store full command
  char** flags = (char *[]){"-i", "-g", "-s", "-o"};
  int i,j;                                               //counters

  if(argc == 1)                              //prints name of the program if there are no other parameters
    printf("%s\n", argv[0]+2);

  else
  {
    if(strncmp(argv[1], flags[0], 2) == 0) // checks if the first command uses the -i flag
    {
      strcat(command, argv[1]+2); //add the name of the input file to command

      for(i = 2; i < argc; i++) //loops until all commands have been added
      {
        for(j = 1; j < 4; j++) //compares to flags
	  if(strncmp(argv[i], flags[j], 2) == 0)
	  {
	    cmd_builder(i, j, command, argv); // calls function to add command
	    break;
	  }

	if(j == 4) // invalid flag
	{
	  printf("Unknown flag: %.2s\nProgram terminated...\n", argv[i]);
	  return 0;
	}
      }

      printf("cmd: %s\n", command); // prints translated command
      system(command); // executes command
    }

    else // no -i flag
    {
      printf("-i flag required...\n");
      return 0;
    }
  }

  return 0;
}

void cmd_builder(int i, int j, char* command, char** argv) // function used to build the command string
{
  char* grep = " | grep ";
  char* sort = " | sort";
  char* output = " > ";

  switch(j)
  {
    case 1:
    strcat(command, grep);
    strcat(command, argv[i]+2);
    break;

    case 2:
    strcat(command, sort);
    break;

    case 3:
    strcat(command, output);
    strcat(command, argv[i]+2);
    break;
  }
}
