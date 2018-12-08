#include <stdio.h>
#include <stdlib.h>

void PermutationBeast(int *array, int i, int length);
void printArr(int *a, int n);

void PermutationBeast(int *array, int i, int length)
{
	int aux;
	if (length == i)
	{
		printArr(array, length);
		return;
	}
	int j = i;
	for (j = i; j < length; j++)
	{
		aux = array[i];
		array[i] = array[j];
		array[j] = aux;
		PermutationBeast(array, i + 1, length);
		aux = array[i];
		array[i] = array[j];
		array[j] = aux;
	}
	return;
}

//Prints the array
void printArr(int *a, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d", a[i]);
	}
	printf("\n");
}


int main (){
	int *a = (int*)malloc(sizeof(int)*3);
	a[0] = 0;
	a[1] = 1;
	a[2] = 2;

	PermutationBeast(a, 0, 3);
}	