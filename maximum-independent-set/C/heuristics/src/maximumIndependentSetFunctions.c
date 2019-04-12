#include "maximumIndependentSet.h"

void allocateMemoryToAGraph(graph **G){
  memalloc(*G,1);
  allocateMemoryAndInitializeASetOfSets(&((*G)->cover));
}

FILE *openFile(char *filepath, char *mode){
  
  FILE *fileP;

  fileP = fopen(filepath, mode);
  assert(fileP);

  return fileP;
}

void getGraphFromDatabase(graph **G, char *filename){

  FILE *fileP;
  char filepath[150];

  strcpy(filepath, _DATABASE_PATH_);
  strcat(filepath, filename);
  
  printf("\ndatabase filepath: %s\n\n", filepath);

  fileP = openFile(filepath, "r");

  getNumbersOfVerticesAndEdges(G, fileP);

  generateVerticesSet(G);

  getEdgesSet(G, fileP);

  generateAdjacencyMatrix(G);

  turnTheGraphIntoItsComplement(G);

  printf("file open !\n");

  fclose(fileP);
}

void getNumbersOfVerticesAndEdges(graph **G, FILE *fileP){

  char s[_STRING_BUFFER_];

  // get ahead reading the file until character p is found
  // in the line: p edges nV nE
  fscanf(fileP, "%s", s);
  while(strcmp(s,"p") != 0){
    fscanf(fileP, "%s", s);
  }

  // read the numbers of Vertices and Edges 
  fscanf(fileP, "%*s %d %d", &((*G)->numberOfVertices), &((*G)->numberOfEdges));

  printf("Original graph:\n");
  printf("number of Vertices --- %d\n", (*G)->numberOfVertices); 
  printf("number of Edges ------ %d\n\n", (*G)->numberOfEdges);
}

void generateVerticesSet(graph **G){
  int i;

  memalloc((*G)->V, (*G)->numberOfVertices)
  memalloc((*G)->invV, (*G)->numberOfVertices)
  for(i=0; i < (*G)->numberOfVertices; i++){
    (*G)->V[i] = i;
    (*G)->invV[i] = i;
  }
}

void getEdgesSet(graph **G, FILE *fileP){
  int i;

  memalloc((*G)->E, (*G)->numberOfEdges)

  for(i=0; fscanf(fileP, "%*s %d %d", &((*G)->E[i][0]), &((*G)->E[i][1])) != EOF; i++){
    (*G)->E[i][0]--;
    (*G)->E[i][1]--;
  }
}

void generateAdjacencyMatrix(graph **G){
  int i;

  memalloc((*G)->adjacencyMatrix, (*G)->numberOfVertices)
  for(i=0; i < (*G)->numberOfVertices; i++){
    (*G)->adjacencyMatrix[i] = calloc((*G)->numberOfVertices, sizeof(*((*G)->adjacencyMatrix[i])));
    assert((*G)->adjacencyMatrix[i]);
    allocCount++;
  }

  for(i=0; i < (*G)->numberOfEdges; i++){
    (*G)->adjacencyMatrix[(*G)->E[i][0]][(*G)->E[i][1]] = 1;
    (*G)->adjacencyMatrix[(*G)->E[i][1]][(*G)->E[i][0]] = 1;
  }
}

void printAdjacencyMatrix(graph **G){
  int i,j;

  printf("\n");
  for(i=0; i < (*G)->numberOfVertices; i++){
    for(j=0; j < (*G)->numberOfVertices; j++){
      printf("%d ", (*G)->adjacencyMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void freeGraph(graph **G){
  freeAdjacencyMatrix(G);
  freeEdges(G);
  freeVertices(G);
  freeCover(G);
  memfree(*G);
}

void freeAdjacencyMatrix(graph **G){
  int i;

  for(i=(*G)->numberOfVertices-1; i >=0; i--){
    memfree((*G)->adjacencyMatrix[i]);
  }

  memfree((*G)->adjacencyMatrix);
}

void freeEdges(graph **G){
  memfree((*G)->E);
}

void freeVertices(graph **G){
  memfree((*G)->V);
  memfree((*G)->invV);
}

void freeCover(graph **G){
  freeSetOfSets(&((*G)->cover));
}

void turnTheGraphIntoItsComplement(graph **G){

  int nV = (*G)->numberOfVertices;
  int nE = (*G)->numberOfEdges;
  int i,j,k;

  freeEdges(G);

  (*G)->numberOfEdges = (int) nV*(nV-1)/2 - nE;
  
  memalloc((*G)->E, (*G)->numberOfEdges)
  
  for(i=0,k=0; i < (*G)->numberOfVertices; i++)
    for(j=i+1; j < (*G)->numberOfVertices; j++){
      if ((*G)->adjacencyMatrix[i][j] == 0){
        (*G)->E[k][0] = i;
        (*G)->E[k][1] = j;
        (*G)->adjacencyMatrix[i][j] = 1;
        (*G)->adjacencyMatrix[j][i] = 1;
	k++;
      }
      else{
        (*G)->adjacencyMatrix[i][j] = 0;
        (*G)->adjacencyMatrix[j][i] = 0;
      }
    }
  
  printf("Complement graph:\n");
  printf("number of Vertices ------------ %d\n", (*G)->numberOfVertices); 
  printf("number of Edges by formule ---- %d\n", (*G)->numberOfEdges);
  printf("number of Edges by counting --- %d\n\n", k);
}

void improvedKellermanHeuristic(graph **G){
/* 
** Following the algorithm designed in the
** "Data reduction, exact, and heuristic for
** clique cover" paper, due to Jens Gramm,
** Jiong Guo, Falk Hüffner and Rolf Niedermeier. 
*/

  int i;
  set *W, *C, *Ck, *U;
  setOfSets *cover; 

  allocateMemoryAndInitializeASet(&W);
  allocateMemoryAndInitializeASet(&U);
  cover = (*G)->cover;

  for(i=0; i < (*G)->numberOfVertices; i++){
    getMinorsVerticesReachedByTheCurrentOne(&W,i,G);

    if (isThisSetEmpty(&W)){
      createANewCliqueAndAppendInTheCover(&cover,&C,i);
    }
    else{
      clearSet(&U);
      C = cover->firstSet;
      do{
        if (isItASubsetOfTheOther(&C, &W)){
	  appendElement(&C, i);
	  expandASetByUnion(&U,&C);

	  if (isTheseSetsEqual(&U, &W))
            break;
	}
        
	C = C->next;
      }while(C != cover->firstSet);
      decreaseSetByDifference(&W, &U);

      while(!isThisSetEmpty(&W)){
        C = getFirstCliqueWithMaxElementsInW(&cover,&W);

        Ck = getIntersectionSet(&C, &W);
        appendElement(&Ck, i);

        appendSet(&cover, &Ck);

        decreaseSetByDifference(&W, &C);
      }
    }
    //printf("v: %3i/%d | cover length: %6d\n", i, (*G)->numberOfVertices, cover->length);
  }

  //printSetOfSets((*G)->cover, "cover");
  //printf("cover generated.\n");
  freeSet(&U);
  freeSet(&W);
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

set *getFirstCliqueWithMaxElementsInW(setOfSets **cover, set **W){

  set *p, *C, *CW;
  int l=0;  

  p = (*cover)->firstSet;
  C = p;
  do{
    CW = getIntersectionSet(&p, W);

    if (CW->length > l){
      C = p;
      l = CW->length;
    }

    freeSet(&CW);
    p = p->next;
  }while( p != (*cover)->firstSet);

  return C;
}

void validateCover(graph **G){

  set *edge;
  int i;
  setOfSets *cover;

  allocateMemoryAndInitializeASet(&edge);

  // verify if all edges are covered //
  cover = (*G)->cover;
  for(i=0; i < (*G)->numberOfEdges; i++){
    // attention: E must be ordered.
    appendElement(&edge, (*G)->E[i][0]);
    appendElement(&edge, (*G)->E[i][1]);

    if (!isThereASetContainingTheCurrentOne(&cover, &edge)){
      printf("\nEdge [%d,%d] is not present in cover.\n",
             (*G)->E[i][0],(*G)->E[i][1]);
      //exit(EXIT_FAILURE);
    }
    //printf("validating edge %6d/%d\n", i, (*G)->numberOfEdges);
    clearSet(&edge);
  }
  freeSet(&edge);

  printf("cover validated.\n");
  // verify if all cliques are cliques
}

void expandCoverToMaximalCliques(graph **G){

  set *clique;

  clique = (*G)->cover->firstSet;
  do{
    maximizeClique(&clique,G);
    clique = clique->next;
  }while(clique != (*G)->cover->firstSet);

  //printf("cover expanded.\n"); 
}

void maximizeClique(set **clique, graph **G){
  element *e;
  int i;

  e = (*clique)->firstElement;
  for(i=0; i < (*G)->numberOfVertices; i++){
    if (i == e->v)
      e = e->next;
    else if (canThisVertexBeIncludedIntoThisClique(i,clique,G)){
      insertElement(i,clique); 
    }
  }
}

int canThisVertexBeIncludedIntoThisClique(int i, set **clique, graph **G){
  element *e;

  e = (*clique)->firstElement;
  do{
    if((*G)->adjacencyMatrix[i][e->v] == 0)
      return FALSE;
    e = e->next;
  }while( e != (*clique)->firstElement);

  return TRUE;
}

void sortCoverByLengthAndSum(graph **G){
  // Bubble sort
  setOfSets *cover;
  set *p, *q, *r;
  int i,j;

  cover = (*G)->cover;
  
  if (cover->length <= 1)
    return;

  for(i=0; i < cover->length; i++){
    p = cover->firstSet;
    for(j=0; j < cover->length-i-1; j++){
      if ((p->length < p->next->length) || 
	  ((p->length == p->next->length) && (p->sum < p->next->sum))){
        q = p->next;

	r = p->previous;
	p->previous = q;
	q->previous = r;
        r->next = q;

	r = q->next;
	q->next = p;
	p->next = r;
        r->previous = p;

        if (cover->firstSet == p)
	  cover->firstSet = q;	
      }
      else p = p->next;
    }
  }
 printf("cover sorted.\n");
}

void generateExtendedCliqueCoverByKellerman(graph **G){
  improvedKellermanHeuristic(G);
  expandCoverToMaximalCliques(G);
  //validateCover(G);
  //printf("cover length: %d\n",(*G)->cover->length);
}

void strengthCurrentCliqueCover(graph **G, int numberOfFormulationsToStrength){
  int i;

  for (i=0; i < numberOfFormulationsToStrength; i++){
    graph *permG;
    setOfSets *newCover;

    generateAPermutatedGraphRepresentation(&permG,G);

    generateExtendedCliqueCoverByKellerman(&permG);

    extractNewCoverWithOriginalLabelling(&newCover, &permG);

    increaseCurrentCoverWithNewCliques(G, &newCover);
 
    printf("increased cover length: %d\n",(*G)->cover->length);

    freeSetOfSets(&newCover); 
    freeGraph(&permG);
  }
  validateCover(G);
}

void generateAPermutatedGraphRepresentation(graph **permG, graph **G){
  
  allocateMemoryToAGraph(permG);
  
  (*permG)->numberOfVertices = (*G)->numberOfVertices;
  (*permG)->numberOfEdges = (*G)->numberOfEdges;

  getPermutatedVerticesFromOriginalGraph(permG,G);

  updateEdgeLabelling(permG,G);

  generateAdjacencyMatrix(permG);
}

void getPermutatedVerticesFromOriginalGraph(graph **permG, graph **G){
  int i,j;

  memalloc((*permG)->V, (*G)->numberOfVertices)
  memalloc((*permG)->invV, (*G)->numberOfVertices)
  
  for(i=0; i < (*G)->numberOfVertices; i++)
    (*permG)->V[i] = (*G)->V[i];

  shuffle((*permG)->V,(*permG)->numberOfVertices, &sfmt);

  for(i=0; i < (*G)->numberOfVertices; i++){
    j = (*permG)->V[i];
    (*permG)->invV[j] = i;
  }
}

void shuffle(int *array, size_t n, sfmt_t *sfmt){
  if (n > 1){
    size_t i;
    for (i=0; i < n-1; i++){
      size_t j = i + sfmt_genrand_uint32(sfmt)/(RAND_MAX_32BITS/(n-i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

void updateEdgeLabelling(graph **permG, graph **G){
  int i, e0, e1;

  memalloc((*permG)->E, (*permG)->numberOfEdges);

  for (i = 0; i < (*G)->numberOfEdges; i++){
    // E must be ordered for the validation stage 
    e0 = (*permG)->V[(*G)->E[i][0]];
    e1 = (*permG)->V[(*G)->E[i][1]];
    
    if (e0 > e1){
      (*permG)->E[i][0] = e1; 
      (*permG)->E[i][1] = e0; 
    }
    else if (e0 < e1){
      (*permG)->E[i][0] = e0; 
      (*permG)->E[i][1] = e1;
    }
    else exit(EXIT_FAILURE);
  }
}
  
void extractNewCoverWithOriginalLabelling(setOfSets **newCover, graph **permG){
  set *clique;
  set *cliqueWithOriginalLabels;
  element *e;

  allocateMemoryAndInitializeASetOfSets(newCover);
  
  clique = (*permG)->cover->firstSet;
  do{
    e = clique->firstElement;
    
    allocateMemoryAndInitializeASet(&cliqueWithOriginalLabels);
    
    do{
      insertElement((*permG)->invV[e->v], &cliqueWithOriginalLabels);
      e = e->next;
    }while(e != clique->firstElement);
    
    appendSet(newCover, &cliqueWithOriginalLabels);
    
    clique = clique->next;
  }while(clique != (*permG)->cover->firstSet);
}

void increaseCurrentCoverWithNewCliques(graph **G, setOfSets **newCover){
  set *newClique,*clique;
  setOfSets *cover;
  int rejectedCliques = 0;

  cover = (*G)->cover;
  newClique = (*newCover)->firstSet;
  do{
 
    if(!isThereASetInDescendingOrderedSetOfSets(&newClique, &cover)){
      allocateMemoryAndInitializeASet(&clique);
      copySet(&clique,&newClique);
      insertSetInSortedSetOfSets(&clique,&cover);
    }
    else rejectedCliques++;//printf("found equal clique.\n");
    newClique = newClique->next;
  }while(newClique != (*newCover)->firstSet);
  printf("rejected cliques: %d/%d - ", rejectedCliques,(*newCover)->length);
}

void allocateMemoryAndInitializeASetOfSets(setOfSets **SS){

  memalloc(*SS,1)
  
  (*SS)->length = 0;
  (*SS)->firstSet = NULL;
}

void allocateMemoryAndInitializeASet(set **S){

  memalloc(*S,1)
  
  (*S)->length = 0;
  (*S)->sum = 0;
  (*S)->firstElement = NULL;
  (*S)->next = NULL;
  (*S)->previous = NULL;
}

void allocateMemoryAndInitializeAnElement(element **e, int i){

  memalloc(*e,1)

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
  (*S)->sum += j;
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

      if (b->v == (*A)->firstElement->v)
        rotateSet(A);
      else 
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

set *getIntersectionSet(set **A, set**B){
/*
**  C = intersection(A,B)
*/
  set *C;
  element *a,*b;
  int indexA, indexB;

  allocateMemoryAndInitializeASet(&C);
  
  if (isThisSetEmpty(A))
    return C;
  
  if (isThisSetEmpty(B))
    return C;

  a = (*A)->firstElement;
  b = (*B)->firstElement;
  
  indexA = 0;
  indexB = 0;
  do{
    if (a->v == b->v){
      appendElement(&C, a->v);
      a = a->next;
      b = b->next;
      indexA++;
      indexB++;
    }
    else if (a->v < b->v){
      a = a->next;
      indexA++;
    }
    else{
      b = b->next;
      indexB++;
    }
  }while((indexA < (*A)->length) &&  (indexB < (*B)->length));

  return C;
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
  element *b, *firstElement;

  firstElement = (*A)->firstElement;

  b = (*B)->firstElement;
  for (indexB=0; (indexB < (*B)->length) && ((*A)->length != 0); indexB++){
    
    if (b->v == (*A)->firstElement->v){
      if ((*A)->firstElement == firstElement)
        firstElement = firstElement->next;

      eraseFirstElementFromSet(A);
    }
    else if ( b->v > (*A)->firstElement->v && 
              b->v <= firstElement->previous->v){
      while (b->v > (*A)->firstElement->v)
        rotateSet(A);
    
      if (b->v == (*A)->firstElement->v)
        eraseFirstElementFromSet(A); 
    }
    else if ( b->v > firstElement->previous->v){
      if ((*A)->length != 0)
        (*A)->firstElement = firstElement;
      
      return;
    }

    b = b->next;
  }

  if ((*A)->length != 0)
    (*A)->firstElement = firstElement;
}

void eraseFirstElementFromSet(set **A){
  element *previous, *next, *target;

  if ((*A)->length != 0)
    target = (*A)->firstElement;    
  else return;

  if ((*A)->length >= 3){
    previous = (*A)->firstElement->previous;
    next = (*A)->firstElement->next;

    previous->next = next;
    next->previous = previous;

    (*A)->firstElement = next;
  }
  else if ((*A)->length == 2){
    next = (*A)->firstElement->next;

    next->next = next;
    next->previous = next;

    (*A)->firstElement = next;
  }
  else if ((*A)->length == 1){
    (*A)->firstElement = NULL;
  }

  (*A)->length--;
  (*A)->sum -= target->v;
  
  memfree(target);
}

void rotateSet(set **S){
  (*S)->firstElement = (*S)->firstElement->next;
}

void  rotateSetOfSets(setOfSets **SS){
  (*SS)->firstSet = (*SS)->firstSet->next;
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

void insertElement(int i, set **clique){
  element *firstElement;

  if ((*clique)->length == 0){
    appendElement(clique,i);
    return;
  }

  if (i <= (*clique)->firstElement->v){
    appendElement(clique,i);
    (*clique)->firstElement = (*clique)->firstElement->previous;
    return;
  }

  if (i > (*clique)->firstElement->previous->v){
    appendElement(clique,i);
    return;
  }

  firstElement = (*clique)->firstElement;
  do{
    rotateSet(clique);
    if (i <= (*clique)->firstElement->v){
      appendElement(clique, i);
      (*clique)->firstElement = firstElement;
      return; 
    }
  }while((*clique)->firstElement != firstElement);
}

void  insertSetInSortedSetOfSets(set **S, setOfSets **SS){
// SS is in descending order.

  if (((*S)->length  > (*SS)->firstSet->length) ||
     (((*S)->length == (*SS)->firstSet->length) &&
      ((*S)->sum    >= (*SS)->firstSet->sum))){
    appendSet(SS,S);
    (*SS)->firstSet = (*SS)->firstSet->previous;
    return;
  }

  if (((*S)->length  < (*SS)->firstSet->previous->length) ||
     (((*S)->length == (*SS)->firstSet->previous->length) &&
      ((*S)->sum     < (*SS)->firstSet->previous->sum))){
    appendSet(SS,S);
    return;
  }

  set *firstSet = (*SS)->firstSet;
  while(((*S)->length  < (*SS)->firstSet->length) ||
       (((*S)->length == (*SS)->firstSet->length) &&
        ((*S)->sum     < (*SS)->firstSet->sum))){
    rotateSetOfSets(SS);
  }
  appendSet(SS,S);

  (*SS)->firstSet = firstSet;
}

int isThereASetInDescendingOrderedSetOfSets(set **S, setOfSets **SS){
  set *q = (*SS)->firstSet;

  do{
    if(((*S)->length  > q->length) ||
      (((*S)->length == q->length) && 
       ((*S)->sum     > q->sum))){
      return FALSE; 
    }

    if(((*S)->length == q->length) &&
       ((*S)->sum == q->sum))
      if(isTheseSetsEqual(S,&q))
        return TRUE;

    q = q->next;
  }while( q != (*SS)->firstSet);

  return FALSE;
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

  if ((*A)->sum != (*B)->sum)
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

int isThereASetContainingTheCurrentOne(setOfSets **A, set **B){

  set *q;
  
  q = (*A)->firstSet;
  do{
    if (isItASubsetOfTheOther(B, &q))
      return TRUE;
    q = q->next;
  }while(q != (*A)->firstSet);

  return FALSE;
}


void printSet(set *S, char *name){

  if (isThisSetEmpty(&S)){
    printf("%s is empty.\n", name);
  }
  else{
    element *p;
	  
    printf("%s - length | sum | content : %2d | %4d | %4d ", name, S->length, S->sum, S->firstElement->v);
    p = S->firstElement->next; 
    while (p != S->firstElement){
      printf("%4d ", p->v);
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

    printf("\n%s :\n", name);

    sprintf(setName, "C%5d", i);
    printSet(SS->firstSet, setName);

    S = SS->firstSet->next;
    while (S != SS->firstSet){
      i++;
      sprintf(setName, "C%5d", i);
      printSet(S, setName);
      S = S->next;
    }
  }
}

void freeSet(set **S){
  clearSet(S);
  memfree(*S);
}

void clearSet(set **S){
  element *p, *n, *tmp;

  if ((*S)->length == 0)
    return;

  if ((*S)->length == 1){
    p = (*S)->firstElement;
    memfree(p);
  }
  else if ((*S)->length > 1){

    p = (*S)->firstElement;
    n = (*S)->firstElement->next;
    
    while(p != n){
      tmp = n;
      n = n->next;
      memfree(tmp);
    }
    
    memfree(p);
  }
  
  (*S)->firstElement = NULL;
  (*S)->length = 0;
  (*S)->sum = 0;
}

void clearSetOfSets(setOfSets **SS){
  set *p,*n,*tmp;

  if ((*SS)->length == 0)
    return;

  if ((*SS)->length == 1){
    p = (*SS)->firstSet;
    
    clearSet(&p);
    memfree(p);
  }
  else if ((*SS)->length > 1){

    p = (*SS)->firstSet;
    n = (*SS)->firstSet->next;
    
    while(p != n){
      tmp = n;
      clearSet(&n);
      n = n->next;
      memfree(tmp);
    }
   
    clearSet(&p); 
    memfree(p);
  }
  
  (*SS)->firstSet = NULL;
  (*SS)->length = 0;
}

void freeSetOfSets(setOfSets **SS){
  clearSetOfSets(SS);
  memfree(*SS);
}
