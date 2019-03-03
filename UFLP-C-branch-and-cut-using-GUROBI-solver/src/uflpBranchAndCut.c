#include "uflpBranchAndCut.h"
#include "separationAlgorithms.h"
#include "errorHandling.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  uflpModel *model;

  initializeFormulationAndVariablesAndParameters(&model);

  gurobiSolver(&model);

  sendResultsToAnOutputFile(&model);

  return OK;
}
