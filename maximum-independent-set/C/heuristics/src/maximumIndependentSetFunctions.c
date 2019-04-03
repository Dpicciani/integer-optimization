#include "maximumIndependentSet.h"

void initializeGlobalGraph(){
  memalloc(G,1)
}

FILE *openFile(char *filepath, char *mode){
  
  FILE *fileP;

  fileP = fopen(filepath, mode);
  assert(fileP);

  return fileP;
}

void getGraphFromDatabase(char *filename){

  FILE *fileP;
  char filepath[150];

  strcpy(filepath, _DATABASE_PATH_);
  strcat(filepath, filename);
  
  printf("%s\n", filepath);

  fileP = openFile(filepath, "r");

  getNumbersOfVerticesAndEdges(fileP);

  generateVerticesSet();

  getEdgesSet(fileP);

  generateAdjacencyMatrix();

  //printAdjacencyMatrix();
 
  turnTheGraphIntoItsComplement();

  printf("file open !\n");

  //freeGraph(G);

  fclose(fileP);
}

void getNumbersOfVerticesAndEdges(FILE *fileP){

  char s[_STRING_BUFFER_];

  // get ahead reading the file until character p is found
  // in the line: p edges nV nE
  fscanf(fileP, "%s", s);
  while(strcmp(s,"p") != 0){
    fscanf(fileP, "%s", s);
  }

  // read the numbers of Vertices and Edges 
  fscanf(fileP, "%*s %d %d", &(G->numberOfVertices), &(G->numberOfEdges));

  printf("nV, nE : %d %d\n", G->numberOfVertices, G->numberOfEdges);
}

void generateVerticesSet(){
  int i;

  memalloc(G->V, G->numberOfVertices)
  for(i=0; i < G->numberOfVertices; i++)
    G->V[i] = i;
}

void getEdgesSet(FILE *fileP){
  int i;

  memalloc(G->E, G->numberOfEdges)

  for(i=0; fscanf(fileP, "%*s %d %d", &(G->E[i][0]), &(G->E[i][1])) != EOF; i++){
    G->E[i][0]--;
    G->E[i][1]--;
    //printf("e %d %d\n", (G->E[i][0]), (G->E[i][1]));
  }
}

void generateAdjacencyMatrix(){
  int i;

  memalloc(G->adjacencyMatrix, G->numberOfVertices)
  for(i=0; i < G->numberOfVertices; i++){
    G->adjacencyMatrix[i] = calloc(G->numberOfVertices, sizeof(*(G->adjacencyMatrix[i])));
    assert(G->adjacencyMatrix[i]);
  }

  for(i=0; i < G->numberOfEdges; i++){
    G->adjacencyMatrix[G->E[i][0]][G->E[i][1]] = 1;
    G->adjacencyMatrix[G->E[i][1]][G->E[i][0]] = 1;
  }
}

void printAdjacencyMatrix(){
  int i,j;

  printf("\n");
  for(i=0; i < G->numberOfVertices; i++){
    for(j=0; j < G->numberOfVertices; j++){
      printf("%d ", G->adjacencyMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void freeGraph(graph *G){

  freeAdjacencyMatrix(G);

  freeEdges(G);

  freeVertices(G);

  free(G);

  printf("Graph G free.\n");
}

void freeAdjacencyMatrix(graph *G){
  int i;

  for(i=G->numberOfVertices; i >=0; i--)
    free(G->adjacencyMatrix[i]); 

  free(G->adjacencyMatrix);
}

void freeEdges(graph *G){
  free(G->E);
}

void freeVertices(graph *G){
  free(G->V);
}

void turnTheGraphIntoItsComplement(){

  int nV = G->numberOfVertices;
  int nE = G->numberOfEdges;
  int i,j,k;

  freeEdges(G);

  G->numberOfEdges = (int) nV*(nV-1)/2 - nE;
  
  memalloc(G->E, G->numberOfEdges)
  
  for(i=0,k=0; i < G->numberOfVertices; i++)
    for(j=i+1; j < G->numberOfVertices; j++){
      if (G->adjacencyMatrix[i][j] == 0){
        G->E[k][0] = i;
        G->E[k][1] = j;
        G->adjacencyMatrix[i][j] = 1;
        G->adjacencyMatrix[j][i] = 1;
	k++;
      }
      else{
        G->adjacencyMatrix[i][j] = 0;
        G->adjacencyMatrix[j][i] = 0;
      }
    }
  printf("nV, nE/k : %d %d/%d\n", G->numberOfVertices, G->numberOfEdges, k);
}

void improvedKellermanHeuristic(graph **G){
/* 
** Following the algorithm designed in the
** "Data reduction, exact, and heuristic for
** clique cover" paper, due to Jens Gramm,
** Jiong Guo, Falk Hüffner and Rolf Niedermeier. 
*/

  int i;
  setOfSets *cover;
  set *W, *C, *U;

  allocateMemoryAndInitializeASetOfSets(&cover);
  allocateMemoryAndInitializeASet(&W);
  allocateMemoryAndInitializeASet(&U);
  
  for(i=0; i < (*G)->numberOfVertices; i++){
    getMinorsVerticesReachedByTheCurrentOne(&W,i,G);

    if (isThisSetEmpty(&W)){
      //printSet(W, "W");

      createANewCliqueAndAppendInTheCover(&cover,&C,i);
    }
    else{
      //printSet(W, "W");
      //printSetOfSets(cover, "cover");
      
      clearSet(&U);
      C = cover->firstSet;
      do{
        if (isItASubsetOfTheOther(&C, &W)){
	  appendElement(&C, i);
	  expandASetByUnion(&U,&C);

	  if (isTheseSetsEqual(&U, &W))
            break;
          //printf("The current set from cover is a subset of W.\n");
	}
	else{
	  //printf("The current set from cover is NOT a subset of W.\n");
	}

	expandASetByUnion(&U,&C);
        
	C = C->next;
      }while(C != cover->firstSet);

      

      printSetOfSets(cover, "cover");
      printSet(U,"covUnion");
    }

    printf("\n");
    //freeSet(&W);
    clearSet(&W);
  }

}

void getMinorsVerticesReachedByTheCurrentOne(set **W, int i, graph **G){

  int j;

  // exit program if W is not found in the right state, empty.
  assert((*W)->length == 0);

  for(j=0; j < i; j++){
    if ((*G)->adjacencyMatrix[i][j] == 1){
      appendElement(W, j);
    }
  }
}

void createANewCliqueAndAppendInTheCover(setOfSets **cover, set **clique, int i){
  allocateMemoryAndInitializeASet(clique);
  appendElement(clique,i);
  appendSet(cover, clique);
}

void allocateMemoryAndInitializeASetOfSets(setOfSets **SS){

  memalloc(*SS,1)
  memalloc((*SS)->firstSet, 1)
  
  (*SS)->length = 0;
  (*SS)->firstSet = NULL;
}

void allocateMemoryAndInitializeASet(set **S){

  memalloc(*S,1)
  memalloc((*S)->firstElement,1)
  memalloc((*S)->next,1)
  memalloc((*S)->previous,1)
  
  (*S)->length = 0;
  (*S)->firstElement = NULL;
  (*S)->next = NULL;
  (*S)->previous = NULL;
}

void allocateMemoryAndInitializeAnElement(element **e, int i){

  memalloc(*e,1)
  memalloc((*e)->next,1)
  memalloc((*e)->previous,1)

  (*e)->v = i;
  (*e)->next = NULL;
  (*e)->previous = NULL;
}

void appendElement(set **S, int j){
  
  element *e;

  allocateMemoryAndInitializeAnElement(&e, j);

  if ((*S)->length == 0){
    (*S)->firstElement = e;
    e->next = e;
    e->previous = e;
  }
  else{
    element *p;
    
    p = (*S)->firstElement->previous;
    p->next = e;
    e->next = (*S)->firstElement;
    (*S)->firstElement->previous = e;
    e->previous = p;
  }
  
  (*S)->length++;
}

void appendSet(setOfSets **SS, set **S){

  if ((*SS)->length == 0){
    (*SS)->firstSet = *S;
    (*S)->next = *S;
    (*S)->previous = *S;
  }
  else{
    set *P;

    P = (*SS)->firstSet->previous;
    P->next = *S;
    (*S)->next = (*SS)->firstSet;
    (*SS)->firstSet->previous = *S;
    (*S)->previous = P;
  }

  (*SS)->length++;
}

void expandASetByUnion(set **A, set **B){
/*
**  A = A U B
*/

  if (isThisSetEmpty(A) && isThisSetEmpty(B))
    return;

  if (!isThisSetEmpty(A) && isThisSetEmpty(B))
    return;
  
  if (isThisSetEmpty(A) && !isThisSetEmpty(B))
    copySet(A,B);

  /* Neither A nor B are empties */

  element *firstElement, *b;
  int indexB;

  firstElement = (*A)->firstElement;

  b = (*B)->firstElement;
  for (indexB=0; indexB < (*B)->length; indexB++){
  
    if( b->v < (*A)->firstElement->v){
      appendElement(A, b->v);

      if (indexB == 0)
        firstElement = (*A)->firstElement->previous;
    }
    else if ( b->v > (*A)->firstElement->v && 
              b->v < firstElement->previous->v){
      while (b->v > (*A)->firstElement->v)
        rotateSet(A);

      appendElement(A, b->v);
    }
    else if ( b->v > firstElement->previous->v){
      (*A)->firstElement = firstElement;
      appendElement(A, b->v);
    }
    else rotateSet(A);

    b = b->next;
  }
  
  (*A)->firstElement = firstElement;
}

void decreaseSetByDifference(set **A, set **B){
/*
**  A = A\B
*/

  if (isThisSetEmpty(A))
    return;

  if (isThisSetEmpty(B))
    return;
  
  int indexB;
  element *a,*b;

  firstElement = (*A)->firstElement;

  a = (*A)->firstElement;
  b = (*B)->firstElement;
  for (indexB=0; indexB < (*B)->length; indexB++){
    
    if (b->v == (*A)->firstElement->v){
      // erase element from A
    }
    else if ( b->v > firstElement->previous->v)
      return;

    b = b->next;
  }
}

void rotateSet(set **S){
  (*S)->firstElement = (*S)->firstElement->next;
}

void copySet(set **A, set **B)
{
  element *p;

  clearSet(A);

  p = (*B)->firstElement;

  if (p == NULL) return;

  do{
    appendElement(A,p->v);
    p = p->next;
  }while(p != (*B)->firstElement);
}

int isThisSetEmpty(set **S){
  if ((*S)->length == 0)
    return TRUE;
  else
    return FALSE;
}

int isThisSetOfSetsEmpty(setOfSets **SS){
  if ((*SS)->length == 0)
    return TRUE;
  else
    return FALSE;
}

int isItASubsetOfTheOther(set **A, set **B){

  if (((*A)->length) > ((*B)->length))
    return FALSE;

  element *a, *b;
  
  int numberOfElementsSearchedFromA = 0;
  int numberOfElementsSearchedFromB = 0;
  
  a = (*A)->firstElement;
  b = (*B)->firstElement;

  do{
    if ((a->v) == (b->v)){
      a = a->next;
      numberOfElementsSearchedFromA++;
    }
    else if ((a->v) < (b->v))
      return FALSE;

    b = b->next;
    numberOfElementsSearchedFromB++;

    if (((*B)->length - numberOfElementsSearchedFromB) < 
	((*A)->length - numberOfElementsSearchedFromA))
      return FALSE;

  }while(numberOfElementsSearchedFromA != (*A)->length);

  return TRUE;
}

int isTheseSetsEqual(set **A, set **B){
  element *a, *b;

  if ((*A)->length != (*B)->length)
    return FALSE;

  a = (*A)->firstElement;
  b = (*B)->firstElement;
  do{
    if (a->v != b->v)
      return FALSE;

    a = a->next;
    b = b->next;
  }while(a != (*A)->firstElement);

  return TRUE;
}

void printSet(set *S, char *name){

  if (isThisSetEmpty(&S)){
    printf("%s is empty.\n", name);
  }
  else{
    element *p;
	  
    printf("%s - length | content : %d | %d ", name, S->length, S->firstElement->v);
    p = S->firstElement->next; 
    while (p != S->firstElement){
      printf("%d ", p->v);
      p = p->next;
    }
    printf("\n");
  }
}

void printSetOfSets(setOfSets *SS, char *name){

  if (isThisSetOfSetsEmpty(&SS)){
    printf("%s is empty.\n", name);
  }
  else{
    int i=0;
    char setName[_STRING_BUFFER_];
    set *S;

    printf("%s :\n", name);

    sprintf(setName, "C%d", i);
    printSet(SS->firstSet, setName);

    S = SS->firstSet->next;
    while (S != SS->firstSet){
      i++;
      sprintf(setName, "C%d", i);
      printSet(S, setName);
      S = S->next;
    }
  }
}

void freeSet(set **S){

  free((*S)->next);
  free((*S)->previous);

  clearSet(S);

  free((*S)->firstElement);
  free(*S);
}

void clearSet(set **S){
  element *p, *n, *tmp;

  if ((*S)->length == 1){
    p = (*S)->firstElement;
    (*S)->firstElement = NULL;
    free(p);
  }
  else if ((*S)->length > 1){

    p = (*S)->firstElement;
    n = (*S)->firstElement->next;
    
    while(p != n){
      tmp = n;
      n = n->next;
      free(tmp);
    }
    
    (*S)->firstElement = NULL;
    free(p);
  }
  
  (*S)->length = 0;
}
