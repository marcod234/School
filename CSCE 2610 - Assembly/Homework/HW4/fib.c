#include <stdio.h>

int fibonacci(int place);

int main (void)
{
  int i, num;

  printf("calculate fibonacci number at  what place?\n");
  scanf("%d", &i);


  num = fibonacci(i);
  printf("%d\n",num);
}

int fibonacci(int place)
{
  if(place==0 || place==1)
    return place;

  else
    return (fibonacci(place-1)+fibonacci(place-2));
}
