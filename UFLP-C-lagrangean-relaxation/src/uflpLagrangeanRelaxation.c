#include "uflpLagrangeanRelaxation.h"
#include "errorHandling.h"

int main(void)
{
  uflpModel *model;
  LLBPtype  *LLBP;
  timeInf   time;

  initializeFormulationAndVariablesAndParameters(&model,&LLBP,&time);

  while (!isStopConditionAchieved(&model,&LLBP))
  {  
    solveLagrangeanLowerBoundProblemByInspection(&model, &LLBP); 
    tryToUpdateTheMaximumLBFoundAndResetSubgradientIterations(&model,&LLBP);
 
    generateAFeasibleSolutionFromTheLagrangeanSolution(&model,&LLBP);
    calculateAndTryToUpdateTheUpperBound(&model,&LLBP);

    adjustStepLengthParameterBasedOnSubgradientIterations(&LLBP);

    calculateSubgradients(&model,&LLBP);
    calculateTheStepSizeForLagrangeanMultipliers(&model,&LLBP);
    updateTheLagrangeanMultipliers(&model,&LLBP);

    updateTotalNumberOfSubgradientIterations(&LLBP);

    displayResults(&model,&LLBP);
  }

  getEndTimeAndDisplayTimeSpent(&time);

  sendResultsToAnOutputFile(&model);

  return OK;
}
