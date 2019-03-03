#include "mochila.h"
#include <stdio.h>
#include <stdlib.h>
#include <numericalRecipes.h>

#define MEMALLOC(x,n,type)  (x) = (type) malloc((n)*sizeof(*(x)));\
			    if (!(x)){ printf(_MALLOC_ERROR_MESSAGE_);\
			                 exit(_MALLOC_ERROR_);} 


knapsackModel *modelMemoryAllocation()
{ 
  knapsackModel * model;
  
  MEMALLOC(model, 1, knapsackModel *)

  return model;
}

FILE *openDatabaseFile(char *filepath, char *mode)
{
  FILE *fp;
  
  fp = fopen(filepath, mode);
  if(!fp){ printf(_OPEN_FILE_ERROR_MESSAGE_);
  	     exit(_OPEN_FILE_ERROR_);}

  return fp;
}

void printModel(knapsackModel *model)
{
  int i;

  printf("\nModel loaded:\n");

  printf("%d %.3f\n", model->n, model->B);

  for(i=0; i<model->n; i+=1)
	  printf("%.3f ", model->c[i]);
  
  printf("\n");
  for(i=0; i<model->n; i+=1)
	  printf("%.3f ", model->b[i]);

  printf("\n");
}

void printSolution(knapsackModel *model)
{
  int i;

  printf("\nSolution found:\nx = ");
  for(i = 0; i < model->n; i+=1)
    printf("%.3f ", model->x[i]);
  printf("\nz = %f\n\n", model->objF);
}

void parametersValidation(knapsackModel *model)
{
  if (model->n < _MINIMUM_NUMBER_OF_OBJECTS_){
    printf(_MINIMUM_OBJECTS_ERROR_MESSAGE_,
	   _MINIMUM_NUMBER_OF_OBJECTS_);
    exit(_NUMBER_OF_OBJECTS_ERROR_);
  }
  else if (model->n > _MAXIMUM_NUMBER_OF_OBJECTS_){
    printf(_MAXIMUM_OBJECTS_ERROR_MESSAGE_,
	   _MAXIMUM_NUMBER_OF_OBJECTS_);
    exit(_NUMBER_OF_OBJECTS_ERROR_);
  }
  else if (model->B < _MINIMUM_KNAPSACK_SPACE_){
    printf(_KNAPSACK_SPACE_ERROR_MESSAGE_,
	   _MINIMUM_KNAPSACK_SPACE_);
    exit(_KNAPSACK_SPACE_ERROR_);
  }
}

knapsackModel *readDatabaseInputFile(char *filepath)
{
  FILE *fp;
  int i;
  knapsackModel *model;
  
  model = modelMemoryAllocation(filepath, "r");

  fp = openDatabaseFile(filepath,"r");
 
  fscanf(fp, "%d", &model->n);
  fscanf(fp, "%lf", &model->B);

  MEMALLOC(model->c, model->n, double *)
  for(i = 0; i<model->n; i+=1)
    fscanf(fp, "%lf", &model->c[i]);

  MEMALLOC(model->b, model->n, double *)
  for(i = 0; i<model->n; i+=1)
    fscanf(fp, "%lf", &model->b[i]);

  MEMALLOC(model->x, model->n, double *);
  for (i = 0; i<model->n; i+=1)
    model->x[i] = 0;

  model->objF = 0;

#ifdef DEBUG
  printModel(model);
#endif

  parametersValidation(model);

  fclose(fp);

  return model;
}

void writeDatabaseOutputFile(knapsackModel *model, char *filepath)
{
  FILE *fp;
  int i;

  fp = openDatabaseFile(filepath, "w");

  fprintf(fp, "%.3f\n", model->objF);

  for(i = 0; i < model->n; i+=1)
	  fprintf(fp, "%.3f ", model->x[i]);
  
  fclose(fp);
}

void knapsackSolver(knapsackModel *model)
{
  double costBenefit[model->n];
  int costBenefitIndices[model->n];
  double availableSpace = model->B;
  int i,j;
  int firstObjectDiscarded = -1;
 
  for (i = 0; i < model->n; i+=1){
    costBenefit[i] = model->c[i]/model->b[i];
    costBenefitIndices[i] = i;
  }
  
  /* sort the cost-benefit variables */
  quickSort(costBenefit, costBenefitIndices, 0, model->n - 1);

#ifdef DEBUG
  printf("\nCost-Benefit result:\n");
  for (i = 0; i < model->n; i+=1)
    printf("%f - %d\n", costBenefit[i], costBenefitIndices[i]);
#endif

  /* generate integer solution*/
  for (i = model->n - 1; i >= 0; i-=1)
  {
    j = costBenefitIndices[i];

    if (model->b[j] <= availableSpace)
    {
      model->x[j] = 1.0;
      model->objF += model->c[j];
      availableSpace -= model->b[j];
    }
    else if (firstObjectDiscarded < 0)
             firstObjectDiscarded = j;
  }

  /* generate the fractional value of the solution */
  if (availableSpace > 0){
    j = firstObjectDiscarded;
    model->x[j] = (double) availableSpace/model->b[j];
    model->objF += model->c[j]*model->x[j];
  }
}



