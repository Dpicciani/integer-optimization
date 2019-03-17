#include <stdio.h>
#include "mochila.h"

int main(int argc, char **argv)
{
  knapsackModel *model;
 
  printf("\nThis program solves the Knapsack Problem.\n\n");

  model = readDatabaseInputFile(_INPUT_FILEPATH_);

  knapsackSolver(model);

  printSolution(model);

  writeDatabaseOutputFile(model, _OUTPUT_FILEPATH_);

  return OK;
}
