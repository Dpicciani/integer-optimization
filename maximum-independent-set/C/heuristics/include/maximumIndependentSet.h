#ifndef _MAXIMUM_INDEPENDENT_SET_HEADER_
#define _MAXIMUM_INDEPENDENT_SET_HEADER_


#include "gurobi_c.h"
#include "SFMT.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define OK	0
#define TRUE	1
#define FALSE	0
#define RAND_MAX_32BITS  4294967295

#define _INPUT_FILEPATH_	"database/input/uflp.in"
#define _OUTPUT_FILEPATH_	"database/output/uflp.out"

//#define _DATABASE_PATH_		"../../../../database/maximum-clique-instances/"
#define _DATABASE_PATH_		"../../../../database/integer_problems/clique-instances/cliques/"

#define _ZERO_GAP_CONDITION_	0.00000001 // 10e-8
#define _STRING_BUFFER_		1000

#define memalloc(x,n)   (x) = malloc((n)*sizeof(*(x)));\
			assert((x));\
                        allocCount++;

#define memfree(x)      free((x));\
                        freeCount++;

/*Macros as Parameters*/
#define _PROBLEM_TYPE_				_MIN_

typedef struct element {
  int v;
  struct element *next;
  struct element *previous;
} element;


typedef struct set {
  int length;
  int sum;  
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
  int	       *invV;
  int	       (*E)[2];
  setOfSets    *cover;
} graph;

/* time information */
typedef struct {
  clock_t begin;
  clock_t end;
} timeInf;

//extern graph *G;

extern int allocCount;
extern int freeCount;
extern sfmt_t sfmt;

void  allocateMemoryToAGraph(graph **G);

void  getGraphFromDatabase(graph **G, char *filename);
FILE *openFile(char *filepath, char *mode);
void  getNumbersOfVerticesAndEdges(graph **G, FILE *fileP);
void  generateVerticesSet(graph **G);
void  getEdgesSet(graph **G, FILE *fileP);
void  generateAdjacencyMatrix(graph **G);
void  printAdjacencyMatrix(graph **G);
void  freeGraph(graph **G);
void  freeAdjacencyMatrix(graph **G);
void  freeEdges(graph **G);
void  freeVertices(graph **G);
void  freeCover(graph **G);
void  turnTheGraphIntoItsComplement(graph **G);

void  improvedKellermanHeuristic(graph **G);
void  getMinorsVerticesReachedByTheCurrentOne(set **S, int i, graph **G);
void  createANewCliqueAndAppendInTheCover(setOfSets **SS, set **S, int i);
set  *getFirstCliqueWithMaxElementsInW(setOfSets **cover, set **W);
set  *createANewCliqueAndAppendElementOfASet(setOfSets **cover, set **C);
void  validateCover(graph **G);
void  expandCoverToMaximalCliques(graph **G);
void  maximizeClique(set **clique, graph **G);
int   canThisVertexBeIncludedIntoThisClique(int i, set **clique, graph **G); 

void  generateExtendedCliqueCoverByKellerman(graph **G);
void  sortCoverByLengthAndSum(graph **G);
void  strengthCurrentCliqueCover(graph **G, int numberOfFormulationsToStrength);
void  generateAPermutatedGraphRepresentation(graph **permG, graph **G);
void  getPermutatedVerticesFromOriginalGraph(graph **permG, graph **G);
void  shuffle(int *array, size_t n, sfmt_t *sfmt);
void  updateEdgeLabelling(graph **permG, graph **G);
void  extractNewCoverWithOriginalLabelling(setOfSets **newCover, graph **permG);
void  increaseCurrentCoverWithNewCliques(graph **G, setOfSets **newCover);

/* set handling */
void  allocateMemoryAndInitializeASetOfSets(setOfSets **SS);
void  allocateMemoryAndInitializeASet(set **S);
void  allocateMemoryAndInitializeAnElement(element **e, int i);
void  appendElement(set **S, int j);
void  appendSet(setOfSets **SS, set **S);
void  expandASetByUnion(set **A, set **B);
set  *getIntersectionSet(set **A, set **B);
void  decreaseSetByDifference(set **A, set **B);
void  eraseFirstElementFromSet(set **A);
void  rotateSet(set **S);
void  copySet(set **A, set **B);
void  insertElement(int i, set **S); 
void  insertSetInSortedSetOfSets(set **S, setOfSets **SS); 
int   isThereASetInDescendingOrderedSetOfSets(set **S, setOfSets **SS);

int   isThisSetEmpty(set **S);
int   isThisSetOfSetsEmpty(setOfSets **SS);
int   isItASubsetOfTheOther(set **A, set**B);
int   isTheseSetsEqual(set **A, set **B); 
int   isThereASetContainingTheCurrentOne(setOfSets **A, set **B);

void  printSet(set *S, char *name);
void  printSetOfSets(setOfSets *S, char *name);
void  freeSet(set **S);
void  clearSet(set **S);
void  clearSetOfSets(setOfSets **SS);
void  freeSetOfSets(setOfSets **SS);
#endif
