#include <stdio.h>

void swap(int v[], int k);
void sort (int v[], int n);

int main (void)
{
  int v[10];
  int choice,k,i;

  printf("Enter 10 numbers\n");

  for(i=0; i<10; i++)
  {
    scanf("%d", &k);
    v[i]=k;
  }


  do
  {
    printf("Swap (1), sort (2), print (3), or exit (4)?\n");
    scanf("%d",&choice);

    switch (choice)
    {
      case 1:
      printf("Enter position 0-9\n");
      scanf("%d",&k);
      swap(v, k);
      break;

      case 2:
      sort(v, 10);
      break;

      case 3:
      for(k=0; k < 10; k++)
	printf("%d ", v[k]);
      printf("\n");
      break;
     }
  }while(choice!=4);

  return 0;
}

void swap(int v[], int k)
{
  int temp;
  temp = v[k];
  v[k] = v[k+1];
  v[k+1] = temp;
}

void sort (int v[], int n)
{
  int i, j;
  for (i=0; i<n; i++)
   for (j=i-1; (j >=0 && v[j] > v[j+1]); j--)
      swap(v,j);
}


