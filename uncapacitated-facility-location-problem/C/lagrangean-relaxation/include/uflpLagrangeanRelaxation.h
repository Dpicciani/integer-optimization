#ifndef _COMBINATORIAL_OPTIMIZATION_HEADER_
#define _COMBINATORIAL_OPTIMIZATION_HEADER_

#include "gurobi_c.h"
#include <stdio.h>
#include <time.h>

#define _INPUT_FILEPATH_	"database/input/uflp.in"
#define _OUTPUT_FILEPATH_	"database/output/uflp.out"

#define _LOG_FILEPATH_		"database/output/uflp.log"

#define _MIN_			0
#define _MAX_			1

#define _ZERO_GAP_CONDITION_	0.00000001 // 10e-8

#define MEMALLOC(x,n)   (x) = malloc((n)*sizeof(*(x)));\
			if (!(x)){\
			  printf(_MALLOC_ERROR_MESSAGE_);\
			  exit(_MALLOC_ERROR_);\
			}

/*Macros as Parameters*/
#define _PROBLEM_TYPE_				_MIN_
#define _SUBGRADIENT_STEP_LENGTH_		2
#define _UPPERBOUND_MULTIPLIER_			1.05
#define _MAXIMUM_SUBGRADIENT_ITERATIONS_	1000
#define _WITHOUT_LB_IMPROVEMENTS_FACTOR_	80.0
#define _MINIMUM_STEP_LENGTH_			0.00000001 // 10e-8
#define _STEP_LENGTH_ATTENUATOR_		0.5

typedef struct {

  int *i;
  int *j;
  int  signal:1;

} Xcomponent;

typedef struct {

  int *i;
  int  signal:1;

} Ycomponent;

typedef struct {

  Xcomponent *X;
  Ycomponent *Y;
  int	     *C;
  int	      inequalitySymbol:2;
  int 	      compacted:1;

} restriction;

typedef struct {

  double **x;
  double  *y;
  double   z;

} solution;

/* uncapacitated facility location problem model */
typedef struct {

  int 		m;
  int 		n;
  double       *F;
  double      **C;
  int 		minOrMax:1;
  solution     *currentFeasibleSol;
  solution     *currentSol;
  solution     *upperBoundSol;
  solution     *lowerBoundSol;
  int		optimumStatus;
} uflpModel;

/* lagrangean lower bound problem type */
typedef struct {

  double	*s;	  // lagrangean multipliers
  double       **lC;      // lagrangean costs
  double        *rlC;     // reduced lagrangean costs
  int           *subGrad; // subgradient vector
  double         f;       // step length for subgradient optimization
  double         T;       // step size for lagrangean multipliers
  double         tau;     // upper bound multiplier for step size T

  int 		 subgradientIterations;
  int 		 totalNumberOfSubgradientIterations;
  int 		 maximumSubgradientIterations;
  int            maximumSubgradientIterationsWithoutLBImprovements;
  int	         minimumStepLength;  
  int 		 status;

} LLBPtype;

/* time information */
typedef struct {
  clock_t begin;
  clock_t end;
} timeInf;


void initializeFormulationAndVariablesAndParameters
     (uflpModel **model, LLBPtype **LLBP, timeInf *time);
 void openDatabaseFileForReading(FILE **databaseFile);
 void readingProblemDimensions(FILE **databaseFile, int *m, int *n);
 void modelMemoryAllocationAndParametersSetting(uflpModel **model, int m, int n);
  void solutionMemoryAllocation(solution **sol, int m, int n);
 void readingFacilityCosts(uflpModel **model, FILE **databaseFile);
 void readingDemandCosts(uflpModel **model, FILE **databaseFile);
 void LLBPmemoryAllocationAndParametersSetting(uflpModel **model, LLBPtype **LLBP);
  void initializeLagrangeanMultipliers(uflpModel **model, LLBPtype **LLBP);
  void initializeSubgradients(uflpModel **model, LLBPtype **LLBP);
 void clockSetting(timeInf *time);

int isStopConditionAchieved(uflpModel **model, LLBPtype **LLBP);
 void verifySubgradientIterationsCondition(LLBPtype **LLBP, int *stop);
 void verifySubgradientVectorNormCondition
      (uflpModel **model, LLBPtype **LLBP, int *stop);
 void verifyZeroOptimalityGapCondition(uflpModel **model, int *stop);
 void verifyStepLengthCondition(LLBPtype **LLBP, int *stop);

void solveLagrangeanLowerBoundProblemByInspection
     (uflpModel **model, LLBPtype **LLBP);
 void calculateLagrangeanCosts(uflpModel **model, LLBPtype **LLBP);
 void calculateReducedLagrangeanCosts(uflpModel **model, LLBPtype **LLBP);
 void getLagrangeanSolution(uflpModel **model, LLBPtype **LLBP);
 void generateSolutionAndLowerBoundBasedOnCurrentLagrangeanSolution
      (uflpModel **model, LLBPtype **LLBP);

void tryToUpdateTheMaximumLBFoundAndResetSubgradientIterations
     (uflpModel **model, LLBPtype **LLBP);
 void copyCurrentSolution(solution **copiedSol, uflpModel **model);

void generateAFeasibleSolutionFromTheLagrangeanSolution
     (uflpModel **model, LLBPtype **LLBP);

void calculateAndTryToUpdateTheUpperBound
     (uflpModel **model, LLBPtype **LLBP);
 void copyCurrentFeasibleSolution
      (solution **copiedSol, uflpModel **model);

void adjustStepLengthParameterBasedOnSubgradientIterations
     (LLBPtype **LLBP);

void calculateSubgradients(uflpModel **model, LLBPtype **LLBP);

void calculateTheStepSizeForLagrangeanMultipliers
     (uflpModel **model, LLBPtype **LLBP);

void updateTheLagrangeanMultipliers(uflpModel **model, LLBPtype **LLBP);

void updateTotalNumberOfSubgradientIterations(LLBPtype **LLBP);

void displayResults(uflpModel **model, LLBPtype **LLBP);
 double calculateOptimalityGap(uflpModel **model);
 int    howManyOpenFacilities(uflpModel **model);

void getEndTimeAndDisplayTimeSpent(timeInf *time);

void sendResultsToAnOutputFile(uflpModel **model);

void openOutputFileForWriting(FILE **outputFile);

#endif
