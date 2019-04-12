#include "maximumIndependentSet.h"

int allocCount = 0;
int freeCount = 0;
sfmt_t sfmt; 

int main(void){
  graph *G;

  sfmt_init_gen_rand(&sfmt, (int) time(NULL));

  allocateMemoryToAGraph(&G);

  //getGraphFromDatabase(&G,"brock200_2.clq");
  //getGraphFromDatabase(&G,"brock400_2.clq");
  //getGraphFromDatabase(&G,"brock800_2.clq");
  //getGraphFromDatabase(&G,"MANN_a81.clq");
  getGraphFromDatabase(&G,"C250.9.clq");
  //getGraphFromDatabase(&G,"keller5.clq");
  //getGraphFromDatabase(&G,"keller6.clq");
  //getGraphFromDatabase(&G,"C4000.5.clq");

  generateExtendedCliqueCoverByKellerman(&G);
 
  sortCoverByLengthAndSum(&G);

  printSetOfSets(G->cover, "cover");

  printf("\n");

  strengthCurrentCliqueCover(&G,200);
  
  freeGraph(&G);
  
  printf("\nProgram reached the end.\n");
  printf("memory allocated --- %d\n", allocCount);
  printf("memory freed ------- %d\n", freeCount);
  printf("memory leak -------- %d\n", allocCount - freeCount);

  return OK;
}

