#ifndef _COMBINATORIAL_OPTIMIZATION_HEADER_
#define _COMBINATORIAL_OPTIMIZATION_HEADER_

#include<stdio.h>

#define DEBUG

#define _INPUT_FILEPATH_		"../database/mochila_frac.in"
#define _OUTPUT_FILEPATH_		"../database/mochila_frac.out"
#define OK				0
#define _MINIMUM_NUMBER_OF_OBJECTS_	3
#define _MAXIMUM_NUMBER_OF_OBJECTS_	1000000
#define _MINIMUM_KNAPSACK_SPACE_	3

#define _MALLOC_ERROR_			-1
#define _MALLOC_ERROR_MESSAGE_		"\nMemory allocation error.\n\n"

#define _OPEN_FILE_ERROR_		-2
#define _OPEN_FILE_ERROR_MESSAGE_ 	"\nOpen file error.\n\n"

#define _NUMBER_OF_OBJECTS_ERROR_	-3
#define _MINIMUM_OBJECTS_ERROR_MESSAGE_	"\nSmall number of objects.(n < %d)\n\n"
#define _MAXIMUM_OBJECTS_ERROR_MESSAGE_	"\nExcessive number of objects.(n > %d)\n\n"

#define _KNAPSACK_SPACE_ERROR_		-4
#define _KNAPSACK_SPACE_ERROR_MESSAGE_  "\nMiddling knapsack space.(B < %d)\n\n"

typedef struct {

  int n;
  double B;
  double *c;
  double *b;
  double *x;
  double objF;

} knapsackModel;

knapsackModel *modelMemoryAllocation();

knapsackModel *readDatabaseInputFile(char *filePath);
void writeDatabaseOutputFile(knapsackModel *, char *filepath);
FILE *openDatabaseInputFile(char *filePath, char *mode);
void parametersValidation(knapsackModel *);

void knapsackSolver(knapsackModel *model);

void printModel(knapsackModel *model);
void printSolution(knapsackModel *model);

#endif
