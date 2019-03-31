#include "maximumIndependentSet.h"


graph *G;

int main(void){

  initializeGlobalGraph();
  getGraphFromDatabase("brock200_2.clq");
  //getGraphFromDatabase("C250.9.clq");
  //getGraphFromDatabase("keller5.clq");

  improvedKellermanHeuristic(&G);

  printf("Program reached the end.\n");
  return OK;
}

