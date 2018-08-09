/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 3600:001                            |
|                           January 22, 2016                          |
|                                                                     |
|    The goal of this program is to prompt the user for any ASCII     |
|    character and then print it out in decimal, hexadecimal, and     |
|    binary.                                                          |
|                                                                     |
|    This program needs the -lm flag to compile                       |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h> //used to call pow

void Dec_to_bin(int dec, char* bin_array); // function prototype that takes an integer and a char pointer

int main(void)
{
  char input; // used to store user input
  char bin[8]; // array that will be used to store the binary number,
	       // array is statically allocated to 8 since in this case since the highest decimal value is 127 which is 8 bits
  int i; // counter

  system("clear");

  printf("Enter an ASCII character: "); //user prompt

  scanf("%c", &input); // get input

  Dec_to_bin(input, bin); // pass input and array address to function

  printf("The ASCII value of %c is: %d (dec) -- %x (hex) -- ", input, input, input); // output decimal and hex info

  for(i = 0; i<8; i++) //prints binary from array
    printf("%c", bin[i]);

  printf(" (bin)\n\n" );

  return 0;
}

void Dec_to_bin(int dec, char* bin_array) // function converts decimal to binary and stores it into the char array
{
  int i;
  int value;

  for(i = 0; i < 8; i++) // loops until all the "bits" are assigned
  {
    value = pow(2,(7-i)); // sets value to the value of the corresponding bit position

    if(dec >= value)
    {
      bin_array[i] = '1'; // places a 1 if the char's decimal value is greater than or equal to the value of the bit position 
      dec -= value;
    }

    else
      bin_array[i] = '0'; // if it is less than then a 0 is placed
  }
}
