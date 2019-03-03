#include "numericalRecipes.h"


void quickSort(double *items, int *indices, int left, int right)
{
  int i, j, aux;
  double x, y;
 
  i = left;
  j = right;
  x = items[(left+right)/2];
 
  do {
    while((items[i] < x) && (i < right))
      i++;
    
    while((x < items[j]) && (j > left))
      j--;
 
    if(i <= j) {
      y = items[i];
      items[i] = items[j];
      items[j] = y;

      aux = indices[i];
      indices[i] = indices[j];
      indices[j] = aux;

      i++; j--;
    }
  } while(i <= j);
 
  if(i < right)
    quickSort(items, indices, i, right);
 
  if(left < j)
    quickSort(items, indices, left, j);
}
