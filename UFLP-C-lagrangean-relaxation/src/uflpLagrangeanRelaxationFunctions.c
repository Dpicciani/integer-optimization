#include "uflpLagrangeanRelaxation.h"
#include "numericalRecipes.h"
#include "errorHandling.h"
#include <stdlib.h>
#include <string.h>
#include <gurobi_c.h>
#include <float.h>
#include <time.h>

void initializeFormulationAndVariablesAndParameters
     (uflpModel **model, LLBPtype **LLBP, timeInf *time)
{
  int m, n;
  FILE *databaseFile;

  openDatabaseFileForReading(&databaseFile);

  readingProblemDimensions(&databaseFile,&m,&n);

  modelMemoryAllocationAndParametersSetting(model, m, n);

  readingFacilityCosts(model, &databaseFile);

  readingDemandCosts(model, &databaseFile);  

  LLBPmemoryAllocationAndParametersSetting(model, LLBP);

  clockSetting(time);
}

void openDatabaseFileForReading(FILE **databaseFile)
{
  (*databaseFile) = fopen(_INPUT_FILEPATH_, "r");
  if (!(*databaseFile)){
    printf(_OPEN_FILE_ERROR_MESSAGE_);
    exit(_OPEN_FILE_ERROR_);
  }
}

void readingProblemDimensions(FILE **databaseFile, int *m, int *n)
{
  fscanf((*databaseFile), "%d", m);
  fscanf((*databaseFile), "%d", n);
}

void modelMemoryAllocationAndParametersSetting(uflpModel **model, int m, int n)
{
  int i;

  /* Memory Allocation */
  MEMALLOC(*model, 1);
  MEMALLOC((*model)->F,m);
  MEMALLOC((*model)->C,m);
  for( i = 0; i < m; i += 1){
    MEMALLOC((*model)->C[i],n);
  }
  
  solutionMemoryAllocation(&((*model)->currentSol), m, n);
  solutionMemoryAllocation(&((*model)->currentFeasibleSol), m, n);
  solutionMemoryAllocation(&((*model)->upperBoundSol), m, n);
  solutionMemoryAllocation(&((*model)->lowerBoundSol), m, n);
  
  /* Model settings */
  (*model)->m = m;
  (*model)->n = n;
  (*model)->minOrMax = _PROBLEM_TYPE_;
  (*model)->upperBoundSol->z = DBL_MAX;
  (*model)->lowerBoundSol->z = -DBL_MAX;
  (*model)->currentSol->z = -DBL_MAX;
}

void solutionMemoryAllocation(solution **sol, int m, int n)
{
  int i;

  MEMALLOC((*sol), 1);
  
  MEMALLOC((*sol)->x, m);
  for(i=0; i < m; i+=1){
    MEMALLOC((*sol)->x[i], n);
  }

  MEMALLOC((*sol)->y, m);
}

void readingFacilityCosts(uflpModel **model, FILE **databaseFile)
{
  int i;

  for(i = 0; i < (*model)->m; i+=1)
    fscanf((*databaseFile), "%*d %lf", &((*model)->F[i]));
}

void readingDemandCosts(uflpModel **model, FILE **databaseFile)
{
  int i,j;

  for (j = 0; j < (*model)->n; j += 1){
    for (i = 0; i < (*model)->m; i += 1)
      if (i==0)
        fscanf((*databaseFile), "%*d %lf ", &((*model)->C[i][j]));
      else
        fscanf((*databaseFile), "%lf ", &((*model)->C[i][j]));
    }
}

void LLBPmemoryAllocationAndParametersSetting
     (uflpModel **model, LLBPtype **LLBP)
{
  int i;

  /* lagrangean multipliers */
  MEMALLOC((*LLBP)->s,(*model)->n);

  /* lagrangean Costs */
  MEMALLOC((*LLBP)->lC,(*model)->m);
  for(i=0; i<(*model)->m; i+=1){
    MEMALLOC((*LLBP)->lC[i],(*model)->n);
  }

  /* reduced lagrangean costs */
  MEMALLOC((*LLBP)->rlC,(*model)->m);

  /* subgradient vector */
  MEMALLOC((*LLBP)->subGrad,(*model)->n);


  /* parameters setting */
  (*LLBP)->f = _SUBGRADIENT_STEP_LENGTH_ ;
  
  (*LLBP)->tau = _UPPERBOUND_MULTIPLIER_;

  (*LLBP)->subgradientIterations = 0;

  (*LLBP)->totalNumberOfSubgradientIterations = 0;

  (*LLBP)->minimumStepLength = _MINIMUM_STEP_LENGTH_;

  (*LLBP)->maximumSubgradientIterations = 
    _MAXIMUM_SUBGRADIENT_ITERATIONS_;
  
  (*LLBP)->maximumSubgradientIterationsWithoutLBImprovements = 
    (*LLBP)->maximumSubgradientIterations/
    _WITHOUT_LB_IMPROVEMENTS_FACTOR_;

  initializeLagrangeanMultipliers(model, LLBP);

  initializeSubgradients(model, LLBP);
}

void initializeLagrangeanMultipliers(uflpModel **model, LLBPtype **LLBP)
{
  /* with zeros */
  int i;
  for (i=0; i<(*model)->n; i+=1)
    (*LLBP)->s[i] = 0;

  /* with minimum strictly positive column values of C */
  int j;
  for (j=0; j<(*model)->n; j+=1){
    i = 0;
   
    if ((*model)->C[i][j] > 0)
      (*LLBP)->s[j] = (*model)->C[i][j];
    else 
      (*LLBP)->s[j] = DBL_MAX;

    for (; i<(*model)->m; i+=1){
      if (((*LLBP)->s[j] > (*model)->C[i][j]) &&
	  ((*model)->C[i][j] > 0 ))
        (*LLBP)->s[j] = (*model)->C[i][j];  
    }
  }
}

void initializeSubgradients(uflpModel **model, LLBPtype **LLBP)
{
  int j;

  for (j=0; j<(*model)->n; j+=1)
    (*LLBP)->subGrad[j] = 1;
}
  
void clockSetting(timeInf *time)
{
  (*time).begin = clock();
}

int isStopConditionAchieved(uflpModel **model, LLBPtype **LLBP)
{
  int stop = 0;

  verifySubgradientIterationsCondition(LLBP, &stop);
  verifySubgradientVectorNormCondition(model, LLBP, &stop);
  verifyZeroOptimalityGapCondition(model, &stop);
  verifyStepLengthCondition(LLBP, &stop);

  return stop;
}

void verifySubgradientIterationsCondition(LLBPtype **LLBP, int *stop)
{
  if ((*stop) == 0 && 
     ((*LLBP)->totalNumberOfSubgradientIterations > 
      (*LLBP)->maximumSubgradientIterations ))
    (*stop) = 1;
}

void verifySubgradientVectorNormCondition
     (uflpModel **model, LLBPtype **LLBP, int *stop)
{
  if ((*stop) == 0 &&
      (iNorm(&((*LLBP)->subGrad), (*model)->n)) == 0)
    (*stop) = 1;
}

void verifyZeroOptimalityGapCondition(uflpModel **model, int *stop)
{
  if ((*stop) == 0 &&
      ((*model)->upperBoundSol->z - (*model)->lowerBoundSol->z
       < _ZERO_GAP_CONDITION_))
    (*stop) = 1;
}

void verifyStepLengthCondition(LLBPtype **LLBP, int *stop)
{
  if ((*stop) == 0 &&
      ((*LLBP)->f < (*LLBP)->minimumStepLength))
    (*stop) = 1;
}

void solveLagrangeanLowerBoundProblemByInspection
     (uflpModel **model, LLBPtype **LLBP)
{
  calculateLagrangeanCosts(model, LLBP);
  
  calculateReducedLagrangeanCosts(model, LLBP);
  
  getLagrangeanSolution(model, LLBP);
  
  generateSolutionAndLowerBoundBasedOnCurrentLagrangeanSolution
    (model, LLBP);
}

void calculateLagrangeanCosts(uflpModel **model, LLBPtype **LLBP)
{
  int i,j;

  for (i=0; i<(*model)->m; i+=1)
    for (j=0; j<(*model)->n; j+=1){
      (*LLBP)->lC[i][j] = (*model)->C[i][j] - (*LLBP)->s[j];
    }
} 

void calculateReducedLagrangeanCosts(uflpModel **model, LLBPtype **LLBP)
{
  int i,j;

  for (i=0; i<(*model)->m; i+=1){
    (*LLBP)->rlC[i] = (*model)->F[i];

    for (j=0; j<(*model)->n;j+=1){
      if ((*LLBP)->lC[i][j] < 0)
        (*LLBP)->rlC[i] += (*LLBP)->lC[i][j];
    }
  }
}

void getLagrangeanSolution(uflpModel **model, LLBPtype **LLBP)
{
  double minimumRLCvalue;	
  int indexOfMinimumRLCvalue;
  int noSolutionWasFound;
  int i;

  minimumRLCvalue = DBL_MAX;
  noSolutionWasFound = 1;
  for (i=0; i<(*model)->m; i+=1){
    if ((*LLBP)->rlC[i] < minimumRLCvalue){
      minimumRLCvalue = (*LLBP)->rlC[i];
      indexOfMinimumRLCvalue = i;
    }

    (*model)->currentSol->y[i] = 0;
    if ((*LLBP)->rlC[i] < 0){
      (*model)->currentSol->y[i] = 1;
      noSolutionWasFound = 0;
    }
  }

  if (noSolutionWasFound)
    (*model)->currentSol->y[indexOfMinimumRLCvalue] = 1;

  (*LLBP)->status = !noSolutionWasFound;
}

void generateSolutionAndLowerBoundBasedOnCurrentLagrangeanSolution
     (uflpModel **model, LLBPtype **LLBP)
{
  int i,j;

  /* generate x solution */
  for (j=0; j<(*model)->n; j+=1)
    for (i=0; i<(*model)->m; i+=1)
      if (((*model)->currentSol->y[i] == 1) && ((*LLBP)->lC[i][j] <= 0))
        (*model)->currentSol->x[i][j] = 1;
      else
        (*model)->currentSol->x[i][j] = 0;

  /* calculate lower bound based on the lagrangean solution */
  (*model)->currentSol->z = 0;
  for (i=0; i<(*model)->m; i+=1)
    if ((*model)->currentSol->y[i] == 1)
      (*model)->currentSol->z += (*LLBP)->rlC[i];

  for (j=0; j<(*model)->n; j+=1)
    (*model)->currentSol->z += (*LLBP)->s[j];
}

void tryToUpdateTheMaximumLBFoundAndResetSubgradientIterations
     (uflpModel **model, LLBPtype **LLBP)
{
  if ((*model)->currentSol->z > (*model)->lowerBoundSol->z){
    copyCurrentSolution(&((*model)->lowerBoundSol), model);
    (*LLBP)->subgradientIterations = 0;
  }
  else
    (*LLBP)->subgradientIterations += 1;
}

void copyCurrentSolution(solution **copiedSol, uflpModel **model)
{
  int i,j;

  /* x */
  for (i=0; i<(*model)->m; i+=1)
    for (j=0; j<(*model)->n; j+=1)
      (*copiedSol)->x[i][j] = (*model)->currentSol->x[i][j];

  /* y */
  for (i=0; i<(*model)->m; i+=1)
    (*copiedSol)->y[i] = (*model)->currentSol->y[i];

  /* z */
  (*copiedSol)->z = (*model)->currentSol->z;
}

void generateAFeasibleSolutionFromTheLagrangeanSolution
     (uflpModel **model, LLBPtype **LLBP)
{
  int i,j;
  int bestFacilityIndex;
  double bestFacilityCost;

  for (j=0; j<(*model)->n;j+=1){
    bestFacilityCost = DBL_MAX;
    
    for (i=0; i<(*model)->m; i+=1){
      if (j==0)
        (*model)->currentFeasibleSol->y[i] = (*model)->currentSol->y[i];

      if (((*model)->currentSol->y[i] == 1) &&
	  ((*model)->C[i][j] < bestFacilityCost)){
         bestFacilityCost = (*model)->C[i][j];     
         bestFacilityIndex = i;
      }

      (*model)->currentFeasibleSol->x[i][j] = 0;
    }

    (*model)->currentFeasibleSol->x[bestFacilityIndex][j] = 1;
  }
}

void calculateAndTryToUpdateTheUpperBound
     (uflpModel **model, LLBPtype **LLBP)
{
  int i,j;

  /* calculate the upper bound */
  (*model)->currentFeasibleSol->z = 0;
  for (j=0; j<(*model)->n; j+=1)
    for (i=0; i<(*model)->m; i+=1){
      if ((*model)->currentFeasibleSol->x[i][j] == 1){
        (*model)->currentFeasibleSol->z += (*model)->C[i][j];
	//break;	
      }
    }

  for (i=0; i<(*model)->m; i+=1)
    if ((*model)->currentFeasibleSol->y[i] == 1)
      (*model)->currentFeasibleSol->z += (*model)->F[i];

  /* try to update the minimum upper bound */
  if ((*model)->currentFeasibleSol->z < (*model)->upperBoundSol->z)
    copyCurrentFeasibleSolution(&((*model)->upperBoundSol), model);
}

void copyCurrentFeasibleSolution
     (solution **copiedSol, uflpModel **model)
{
  int i,j;

  /* x */
  for (i=0; i<(*model)->m; i+=1)
    for (j=0; j<(*model)->n; j+=1)
      (*copiedSol)->x[i][j] = (*model)->currentFeasibleSol->x[i][j];

  /* y */
  for (i=0; i<(*model)->m; i+=1)
    (*copiedSol)->y[i] = (*model)->currentFeasibleSol->y[i];

  /* z */
  (*copiedSol)->z = (*model)->currentFeasibleSol->z;
}

void adjustStepLengthParameterBasedOnSubgradientIterations
     (LLBPtype **LLBP)
{
  if ((*LLBP)->subgradientIterations == 
      (*LLBP)->maximumSubgradientIterationsWithoutLBImprovements){
    (*LLBP)->f *= _STEP_LENGTH_ATTENUATOR_;
    (*LLBP)->subgradientIterations = 0;
  }
}   

void calculateSubgradients(uflpModel **model, LLBPtype **LLBP)
{
  int i,j;

  for (j=0; j<(*model)->n; j+=1){
    (*LLBP)->subGrad[j] = 1;
    for (i=0; i<(*model)->m; i+=1)
      (*LLBP)->subGrad[j] -= (*model)->currentSol->x[i][j];
  } 
}

void calculateTheStepSizeForLagrangeanMultipliers
     (uflpModel **model, LLBPtype **LLBP)
{
  (*LLBP)->T = ((*LLBP)->tau)*((*model)->upperBoundSol->z); 
  (*LLBP)->T -= (*model)->currentSol->z;
  (*LLBP)->T *= (*LLBP)->f;
  (*LLBP)->T /= (double) iDot(&((*LLBP)->subGrad), (*model)->n);
}

void updateTheLagrangeanMultipliers(uflpModel **model, LLBPtype **LLBP)
{
  int j;

  for (j=0; j<(*model)->n; j+=1)
    (*LLBP)->s[j] += ((*LLBP)->T)*((*LLBP)->subGrad[j]);
}

void updateTotalNumberOfSubgradientIterations(LLBPtype **LLBP)
{
  (*LLBP)->totalNumberOfSubgradientIterations += 1;
}

void displayResults(uflpModel **model, LLBPtype **LLBP)
{
  printf("UB: %14.5lf | LB: %14.5lf ",(*model)->upperBoundSol->z
		                      ,(*model)->lowerBoundSol->z );
  printf("| optGap: %7.4lf %% ", calculateOptimalityGap(model));
  printf("| iter: %4d ", (*LLBP)->totalNumberOfSubgradientIterations);
  printf("| openF: %2d ", howManyOpenFacilities(model));
  printf("| f: %.6lf ", (*LLBP)->f);
  printf("| cFeaSol: %12.3lf ", (*model)->currentFeasibleSol->z);
  printf("| cLagSol: %12.3lf ", (*model)->currentSol->z);
  printf("| lagSolFound: %d ", (*LLBP)->status); 

  printf("\n");
}

double calculateOptimalityGap(uflpModel **model)
{
  double gapOpt;

  gapOpt  = (*model)->upperBoundSol->z;
  gapOpt -= (*model)->lowerBoundSol->z;
  gapOpt *= 100;
  gapOpt /= (*model)->lowerBoundSol->z;

  return gapOpt;
}

int howManyOpenFacilities(uflpModel **model)
{
  int open, i;

  for (i=0,open=0; i<(*model)->m; i+=1)
    open += (*model)->lowerBoundSol->y[i];

  return open;
}

void getEndTimeAndDisplayTimeSpent(timeInf *time)
{
  double timeSpentInSec;

  (*time).end = clock();

  timeSpentInSec = (double) ((*time).end - (*time).begin)/CLOCKS_PER_SEC;

  printf("\nTime spent: %9.4lf sec\n\n", timeSpentInSec);
}

void sendResultsToAnOutputFile(uflpModel **model)
{
  FILE *outputFile;
  int i,j;

  openOutputFileForWriting(&outputFile);

  /* Objective function value */
  fprintf(outputFile,"%lf\n" ,(*model)->upperBoundSol->z);

  /* X variable */
  for(i = 0; i < (*model)->m; i += 1){
    for(j = 0; j < (*model)->n; j += 1)
      fprintf(outputFile,"%d " ,(int) (*model)->upperBoundSol->x[i][j]);
    fprintf(outputFile,"\n");
  }

  fprintf(outputFile, "\n");

  /* Y variable */
  for (i = 0; i < (*model)->m; i += 1)
    fprintf(outputFile,"%d " ,(int) (*model)->upperBoundSol->y[i]);

  fclose(outputFile);
}

void openOutputFileForWriting(FILE **outputFile)
{
  (*outputFile) = fopen(_OUTPUT_FILEPATH_, "w");
  if (!(*outputFile)){
    printf(_OPEN_FILE_ERROR_MESSAGE_);
    exit(_OPEN_FILE_ERROR_);
  }
}


