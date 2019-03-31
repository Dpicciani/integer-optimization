#ifndef _MAXIMUM_INDEPENDENT_SET_HEADER_
#define _MAXIMUM_INDEPENDENT_SET_HEADER_


#include "gurobi_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define OK	0

#define _INPUT_FILEPATH_	"database/input/uflp.in"
#define _OUTPUT_FILEPATH_	"database/output/uflp.out"

#define _DATABASE_PATH_		"../../../../database/integer_problems/clique-instances/cliques/"

#define _ZERO_GAP_CONDITION_	0.00000001 // 10e-8
#define _STRING_BUFFER_		1000

#define memalloc(x,n)   (x) = malloc((n)*sizeof(*(x)));\
			assert((x));

/*Macros as Parameters*/
#define _PROBLEM_TYPE_				_MIN_

typedef struct element {
  int v;
  struct element *next;
  struct element *previous;
} element;


typedef struct set {
  int	length;
  element *firstElement;
  struct set *next;
  struct set *previous;
} set;

typedef struct setOfSets {
  int length;
  set *firstSet;
} setOfSets;

/* graph structure*/
typedef struct {
  int 		numberOfVertices;
  int		numberOfEdges;
  int	      **adjacencyMatrix;
  int	       *V;
  int	       (*E)[2];
} graph;

/* time information */
typedef struct {
  clock_t begin;
  clock_t end;
} timeInf;

extern graph *G;

void initializeGlobalGraph();

void getGraphFromDatabase(char *filename);
FILE *openFile(char *filepath, char *mode);
void getNumbersOfVerticesAndEdges(FILE *fileP);
void generateVerticesSet();
void getEdgesSet(FILE *fileP);
void generateAdjacencyMatrix();
void printAdjacencyMatrix();
void freeGraph(graph *G);
void freeAdjacencyMatrix(graph *G);
void freeEdges(graph *G);
void freeVertices(graph *G);
void turnTheGraphIntoItsComplement();

void improvedKellermanHeuristic(graph **G);
void getMinorsVerticesReachedByTheCurrentOne(set **S, int i, graph **G);

/* set handling */
void allocateMemoryAndInitializeASetOfSets(setOfSets **SS);
void allocateMemoryAndInitializeASet(set **S);
void allocateMemoryAndInitializeAnElement(element **e, int i);
void appendElement(set **S, int j);
void appendSet(setOfSets **SS, set **S);
void printSet(set *S, char *setName);
void freeSet(set **S);
void clearSet(set **S);
#endif
