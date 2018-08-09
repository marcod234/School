#include <stdio.h>

int main(void)
{
  int lines, stars, i, j;
  char c;

  printf("Enter a positive number: ");
  scanf("%d", &lines);

  for(i=1; i <= lines; i++)
  {
    stars=i;
    for(j=1; j <= lines; j++)
    {
      if(j <= (lines-stars))
	 c = ' ';
      else
         c = '*';

      putchar(c);
    }
    putchar('\n');
  }

  return 0;
}
