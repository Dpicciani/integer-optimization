#ifndef _COMBINATORIAL_OPTIMIZATION_HEADER_
#define _COMBINATORIAL_OPTIMIZATION_HEADER_

#include <stdio.h>
#include "gurobi_c.h"


//#define DEBUG01
//#define DEBUG11
//#define DEBUG12
//#define DEBUG13
//#define DEBUG14
//#define DEBUG15


#define _INPUT_FILEPATH_	"database/input/uflp.in"
#define _OUTPUT_FILEPATH_	"database/output/uflp.out"

#define _LP_FILEPATH_		  "buffer/uflpModel.lp"
#define _LP_OUTPUT_FILEPATH_	  "buffer/uflpGurobiModel.lp"
#define _LP_LINE_LIMIT_		  75
#define _LP_STRING_BUFFER_LENGTH_ 50

#define _LOG_FILEPATH_		"database/output/uflp.log"

#define _MIN_			0
#define _MAX_			1

#define _NEGATIVE_		0
#define _POSITIVE_		1

#define _INFEASIBILITY_		0
#define _BOUND_			1
#define _OPTIMALITY_		2

#define MEMALLOC(x,n)   (x) = malloc((n)*sizeof(*(x)));\
			if (!(x)){\
			  printf(_MALLOC_ERROR_MESSAGE_);\
			  exit(_MALLOC_ERROR_);\
			}

/*Macros as Parameters*/
#define _PROBLEM_TYPE_				_MIN_
#define NUMBER_OF_ORIGINAL_RESTRICTIONS()	n + (2*n*m) + m	



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

typedef struct {

  int 		m;
  int 		n;
  double       *F;
  double      **C;
  int 		minOrMax:1;
  solution     *sol;
  int		optimumStatus;
} uflpModel;

void initializeFormulationAndVariablesAndParameters(uflpModel **model);
void openDatabaseFileForReading(FILE **databaseFile);
void readingProblemDimensions(FILE **databaseFile, int *m, int *n);
void modelMemoryAllocationAndParametersSetting(uflpModel **model, int m, int n);
void readingFacilityCosts(uflpModel **model, FILE **databaseFile);
void readingDemandCosts(uflpModel **model, FILE **databaseFile);
void generateLPfileWithOriginalFormulation(uflpModel **model);
void openLPfileForWriting(FILE **lpFile);

void gurobiSolver(uflpModel **model);
void errorMsg(const int er, const char *msg, GRBenv *env);

void sendResultsToAnOutputFile(uflpModel **model);
void openOutputFileForWriting(FILE **outputFile);

#endif
