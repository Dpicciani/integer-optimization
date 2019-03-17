#include "separationAlgorithms.h"
#include "errorHandling.h"
#include <stdlib.h>

void threeClientSeparation(int m, 
		           int n, 
			   solution **sol, 
			   threeClientRestrictionList **TCrList,
			   int *TCrLength)
{
  int i,j,fractionaryClients = 0;
  int j1, j2, j3;
  Jfractional *J1, *J2, *J3,
	           *J2_, *J3_;
  threeClientRestriction *threeClientR; 
  int    oldTCrLength = *TCrLength;
  int    stop = 0;
  int    IndexOfMaximumViolatedEquation;
  double maximumViolation;
  double qtt;
  int   *IXf, *IXfWD;
  int    size;

  Jfractional *Jfbase = NULL;

  computeJfAndEf(m, n, sol, &Jfbase, &fractionaryClients);

  //printJfEf(Jfbase);

  if (fractionaryClients < 3){return;}

  /* Separation algorithm */
  
  J1 = Jfbase;
  J2 = J1->next;
  J3 = J2->next;

  J2_ = J2;
  J3_ = J3;

  int permCount = 0;
  while (!stop){
    permCount +=1;
    j1 = J1->index;
    j2 = J2->index;
    j3 = J3->index;
    IXf = NULL;
    
    threeClientMemoryAllocation(&threeClientR);

    threeClientJSetting(&threeClientR, j1, j2, j3);

    getIndicesOfFractionaryXBasedOnCurrentT(&IXf,J1,J2,J3);

    cleanDuplicatedElements(&IXfWD, &IXf, degreeSum(J1,J2,J3), &size);
    
    for (j=0, i=IXfWD[j]; j < size; j+=1, i=IXfWD[j]){
   
      /* calculate the four quantities */
      qtt = 2*((*sol)->x[i][j1] + 
	       (*sol)->x[i][j2] - 
	       (*sol)->y[i] ); 
      maximumViolation = qtt;
      IndexOfMaximumViolatedEquation = 0;

      qtt = 2*((*sol)->x[i][j1] + 
	       (*sol)->x[i][j3] - 
	       (*sol)->y[i] ); 
      if ( maximumViolation < qtt ){
        maximumViolation = qtt;
        IndexOfMaximumViolatedEquation = 1;}

      qtt = 2*((*sol)->x[i][j2] + 
	       (*sol)->x[i][j3] - 
	       (*sol)->y[i] ); 
      if ( maximumViolation < qtt ){
        maximumViolation = qtt;
        IndexOfMaximumViolatedEquation = 2;}
      
      qtt = (*sol)->x[i][j1] + 
            (*sol)->x[i][j2] + 
	    (*sol)->x[i][j3] - 
	    (*sol)->y[i]; 
      if ( maximumViolation < qtt ){
        maximumViolation = qtt;
        IndexOfMaximumViolatedEquation = 3;}
      
      if (maximumViolation > 0){
        switch (IndexOfMaximumViolatedEquation){
        case 0:
	      addIndexToSetS(&(threeClientR->S12), i);
              threeClientR->Slength[0] += 1;
	      //printf("| i in S12");
	      break;
        case 1:
	      addIndexToSetS(&(threeClientR->S13), i);
              threeClientR->Slength[1] += 1;
              //printf("| i in S13");
              break;
        case 2:
	      addIndexToSetS(&(threeClientR->S23), i);
              threeClientR->Slength[2] += 1;
              //printf("| i in S23");
              break;
        case 3:
	      addIndexToSetS(&(threeClientR->S123), i);
              threeClientR->Slength[3] += 1;
              //printf("| i in S123");
	      break;
        }

	threeClientR->violationDegree += maximumViolation;
      }
    }
    
    createTokens(&threeClientR);
   
    analyzeAcceptanceAndUpdateRestrictionList(&threeClientR, TCrList, TCrLength);

    updatePointersForPermutationOfTripleClients(&stop,&J1,&J2,&J2_,&J3,&J3_);

    if ((*TCrLength - oldTCrLength) >= _MAXIMUM_NUMBER_OF_CUTS_PER_PASSAGE_ )
      stop = 1;
  }
}

void computeJfAndEf(int m, int n, solution **sol, Jfractional **Jfbase, int *fractionaryClients)
{
  int i,j;

  Jfractional *Jf = NULL;
  Xfractional *Xf;

  /* compute Jf and Ef */

  for (j=0; j<n; j+=1){
    for (i=0; i<m; i+=1){
      if (((*sol)->x[i][j] > 0)&&((*sol)->x[i][j] < 1)){
        if ((*Jfbase) == NULL){
	  *fractionaryClients += 1;
	  JfracMemoryAllocation(Jfbase);
	  XfracMemoryAllocation(&((*Jfbase)->Xfrac));
	  (*Jfbase)->index = j;
	  Jf = *Jfbase;
	  Xf = Jf->Xfrac;
	  Xf->index = i;
	  Jf->degree += 1;
	}
	else if (Jf->index != j){
	  *fractionaryClients += 1;
	  JfracMemoryAllocation(&(Jf->next));
	  Jf = Jf->next;
	  Jf->index = j;
	  Jf->degree += 1;
	  XfracMemoryAllocation(&(Jf->Xfrac));
	  Xf = Jf->Xfrac;
	  Xf->index = i;
	  
	}	
        else if (Xf->index != i){
	  XfracMemoryAllocation(&(Xf->next));
	  Xf = Xf->next;
	  Xf->index = i;
	  Jf->degree += 1;
	}
      }
    }
  }
}

void JfracMemoryAllocation(Jfractional **Jf)
{
  MEMALLOC((*Jf),1);
  (*Jf)->next = NULL;
  (*Jf)->Xfrac = NULL;
  (*Jf)->degree = 0;
}

void XfracMemoryAllocation(Xfractional **Xf)
{
  MEMALLOC((*Xf),1);
  (*Xf)->next = NULL;
}

void threeClientJSetting(threeClientRestriction **threeClientR, int j1, int j2, int j3)
{
  (*threeClientR)->J[0] = j1;
  (*threeClientR)->J[1] = j2;
  (*threeClientR)->J[2] = j3;
}

void getIndicesOfFractionaryXBasedOnCurrentT
     (int **IXf,Jfractional *J1, Jfractional *J2, Jfractional *J3)
{
  int i = 0;

  MEMALLOC(*IXf, degreeSum(J1,J2,J3));
 
  getAllXfValuesFromJfractional(&i, IXf, J1);
  getAllXfValuesFromJfractional(&i, IXf, J2);
  getAllXfValuesFromJfractional(&i, IXf, J3);
}

void getAllXfValuesFromJfractional(int *i, int **IXf, struct Jfractional *J)
{
  Xfractional *Xf;
  Xf = J->Xfrac;
  while (Xf != NULL){
    //printf("index = %d\n", Xf->index);
    (*IXf)[*i] = Xf->index;
    *i += 1;
    Xf = Xf->next;
  }
}

int degreeSum(struct Jfractional *J1, 
	      struct Jfractional *J2, 
	      struct Jfractional *J3)
{
  return J1->degree + J2->degree + J3->degree;
}

void cleanDuplicatedElements(int **I, int **IXf, int sizeIn, int *sizeOut)
{
  int i,j,k;
  int *Iaux;
  
  // copying IXf to not alter the extern array (desnecessary?)
  MEMALLOC(Iaux,sizeIn);
  for (i=0; i < sizeIn; i+=1){
    Iaux[i] = (*IXf)[i];
  }

  // eliminating repeated elements
  for (i=0; i < sizeIn;i+=1){
    for (j=i+1; j < sizeIn;){
      if (Iaux[j] == Iaux[i]){
        for(k=j; k < sizeIn; k+=1){
          Iaux[k] = Iaux[k+1];
        }
        sizeIn -= 1;
      }
      else{
        j += 1;
      }
    }
  }

  // recording schrinked array
  MEMALLOC(*I,sizeIn);
  for (i=0; i < sizeIn; i+=1){
    (*I)[i] = Iaux[i];
  }
  *sizeOut = sizeIn;
}

void updatePointersForPermutationOfTripleClients(int          *stop,
		                                 Jfractional **J1,
		                                 Jfractional **J2,
		                                 Jfractional **J2_,
		                                 Jfractional **J3,
		                                 Jfractional **J3_)
{
    if ((((*J1)->next->next->next != NULL)  ||
         ((*J2_)->next->next != NULL)       ||
         ((*J3_)->next != NULL))              )
    {
      if (((*J3)->next == NULL) && ((*J3_)->next != NULL))
      {
        (*J3) = (*J3_)->next;
        (*J3_) = (*J3_)->next;

        if (((*J2)->next->next == NULL) && ((*J2_)->next->next != NULL)){
          (*J2) = (*J2_)->next;
          (*J2_) = (*J2_)->next;

	  if ((*J1)->next->next->next != NULL){
	    (*J1) = (*J1)->next;
	  }
        }
        else if (((*J2)->next->next != NULL) && ((*J2_)->next->next != NULL)){
          (*J2) = (*J2)->next;
        }
      }
      else if (((*J3)->next != NULL) && ((*J3_)->next != NULL)){
        (*J3) = (*J3)->next;
      }
      else if ((*J2)->next->next == NULL){
        *J1 = (*J1)->next;
        *J2_ = (*J1)->next;
        *J3_ = (*J2_)->next;
        *J2 = *J2_;
        *J3 = *J3_;
      }
    }
    else *stop = 1;
}


void separationTest()
{
  solution *sol;
  int m = 5,
      n = 14,
      i,j;
  threeClientRestrictionList *TCrList;

  /* allocate memory for current variables */
  MEMALLOC(sol,1);
  MEMALLOC(sol->y,m);
  for (i=0; i < m; i+=1)
    sol->y[i] = 0;
  MEMALLOC(sol->x,m);
  for (i=0;i<m;i+=1){
    MEMALLOC(sol->x[i],n);
    for (j=0;j<n;j+=1)
      sol->x[i][j] = 0;
  }
  //MEMALLOC(TCrList,1);
  TCrList = NULL;

  /* fractionary points */
  sol->x[4][1] = 0.5;
  sol->x[3][2] = 0.3;
  sol->x[4][3] = 0.2;
  sol->x[4][4] = 0.3;
  sol->x[0][5] = 0.2;
  sol->x[2][5] = 0.7;
  sol->x[4][5] = 0.4;
  
  sol->y[0] = 0.5;
  sol->y[2] = 0.5;
  sol->y[3] = 0.51;
  sol->y[4] = 0.51;


  /* show X variable in command line */
  printf("\n\n--Starting 3-client separation\n");
  for (i=0;i<m;i+=1){
    printf("\n");
    for (j=0;j<n;j+=1)
      printf("%4.2lf ", sol->x[i][j]);
  }
  printf("\n");

  /* testing of the separation algorithm */
  //threeClientSeparation(m,n,&sol, &TCrList);

  if (TCrList != NULL)
    printf("\nTEM RESTRICAO\n\n");
  else
    printf("\nNAO TEM RESTRICAO\n\n");
}

void printJfEf(Jfractional *Jfbase)
{
  int i,j;
  Jfractional *Jf;
  Xfractional *Xf;

  Jf = Jfbase;

  printf("\n");

  while (Jf != NULL){
    Xf = Jf->Xfrac;
    j = Jf->index;
    
    while (Xf != NULL){
      i = Xf->index;

      printf("(i*,j*) = (%d,%d)\n", i,j);
      
      Xf = Xf->next;
    }
    Jf = Jf->next;
  }
}

void threeClientMemoryAllocation(threeClientRestriction **threeClientR)
{
  MEMALLOC(*threeClientR,1);

  (*threeClientR)->S12 = NULL;
  (*threeClientR)->S13 = NULL;
  (*threeClientR)->S23 = NULL;
  (*threeClientR)->S123 = NULL;

  (*threeClientR)->Slength[0] = 0;
  (*threeClientR)->Slength[1] = 0;
  (*threeClientR)->Slength[2] = 0;
  (*threeClientR)->Slength[3] = 0;


  (*threeClientR)->J[0] = -1;
  (*threeClientR)->J[1] = -1;
  (*threeClientR)->J[2] = -1;

  (*threeClientR)->token1 = 0;
  (*threeClientR)->token2 = 0;
  (*threeClientR)->token3 = 0;
  
  (*threeClientR)->violationDegree = 0;
}

void addIndexToSetS(Stype **S, int index)
{
  Stype *aux;

  if (*S == NULL){
    MEMALLOC(*S,1);
    (*S)->next = NULL;
    (*S)->index = index;
  }
  else{
    aux = *S;	 
  
    while (aux->next != NULL)
      aux = aux->next;
  
    MEMALLOC(aux->next,1);
    aux = aux->next;
    aux->next = NULL;
    aux->index = index;
  }
}


void createTokens(threeClientRestriction **threeClientR)
{
  createToken1(threeClientR);
  createToken2(threeClientR);
  createToken3(threeClientR);
}

void createToken1(threeClientRestriction **threeClientR)
{  
  Stype *S;

  S = (*threeClientR)->S12;
  addSumOfIndicesToToken(&((*threeClientR)->token1), S);

  S = (*threeClientR)->S13;
  addSumOfIndicesToToken(&((*threeClientR)->token1), S);

  S = (*threeClientR)->S23;
  addSumOfIndicesToToken(&((*threeClientR)->token1), S);
  
  S = (*threeClientR)->S123;
  addSumOfIndicesToToken(&((*threeClientR)->token1), S);
}

void addSumOfIndicesToToken(int *indicesSum, Stype *S)
{
  if (S != NULL){
    *indicesSum += S->index;

    while (S->next != NULL){
      S = S->next;
      *indicesSum += S->index;
    }
  }
}

void createToken2(threeClientRestriction **threeClientR)
{
  (*threeClientR)->token2 = (*threeClientR)->J[0] + 
	                    (*threeClientR)->J[1] +
	                    (*threeClientR)->J[2];
}
void createToken3(threeClientRestriction **threeClientR)
{
  (*threeClientR)->token3 = (*threeClientR)->Slength[0] + 
	                    (*threeClientR)->Slength[1] +
	                    (*threeClientR)->Slength[2] +
	                    (*threeClientR)->Slength[3];
}

void analyzeAcceptanceAndUpdateRestrictionList
     (threeClientRestriction **threeClientR, 
      threeClientRestrictionList **TCrList,
      int *TCrLength)
{
  threeClientRestriction     *currentTC;
  threeClientRestrictionList *auxList;
  threeClientRestrictionList *lastR;

  MEMALLOC(lastR,1);
  MEMALLOC(lastR->threeClient,1);
  lastR->threeClient = NULL;

  if ((*threeClientR)->Slength[0] == 0 ||
      (*threeClientR)->Slength[1] == 0 ||
      (*threeClientR)->Slength[2] == 0 ){
    freeThreeClientRestriction(threeClientR);
    free(lastR);
    return; // unacceptable restriction
  }

  auxList = (*TCrList);

  while (auxList != NULL){
    currentTC = auxList->threeClient;

    if ((*threeClientR)->token1 == currentTC->token1 &&
        (*threeClientR)->token2 == currentTC->token2 &&
        (*threeClientR)->token3 == currentTC->token3 ){
      freeThreeClientRestriction(threeClientR);
      freeThreeClientRestriction(&(lastR->threeClient));
      free(lastR);
      return; // unacceptable restriction
    }

    if (auxList->next == NULL)
      lastR = auxList;

    auxList = auxList->next;
  }

  if (lastR->threeClient != NULL){
    MEMALLOC(lastR->next,1);
    lastR = lastR->next;
    MEMALLOC(lastR->threeClient,1);
    lastR->next = NULL;
  }
  else{
    *TCrList = lastR;
  }

  lastR->threeClient = (*threeClientR);
  *TCrLength += 1;
}

void freeStypeMemory(Stype **S)
{
  Stype *aux;

  if (*S != NULL){
    while ((*S)->next != NULL){
      aux = *S;
      *S = (*S)->next;
      free(aux);
    }

    free(*S);
  }
}

void freeThreeClientRestriction(threeClientRestriction **TCR)
{
  if (*TCR != NULL){
    freeStypeMemory(&((*TCR)->S12));
    freeStypeMemory(&((*TCR)->S13));
    freeStypeMemory(&((*TCR)->S23));
    freeStypeMemory(&((*TCR)->S123));

    free(*TCR);
  }
}

void getTheMostViolatedRestrictionAndFreeTheOthers
     (threeClientRestriction **threeClientR, threeClientRestrictionList **TCrList)
{
  threeClientRestrictionList *aux;//, *garbage;
 
  if (*TCrList == NULL)
    return;

  aux = *TCrList;
  *threeClientR = (*TCrList)->threeClient;
  
  while( aux != NULL){
    if (aux->threeClient->violationDegree > (*threeClientR)->violationDegree ){
      *threeClientR = aux->threeClient;
      //garbage = aux;
      aux = aux->next;
      //freeThreeClientRestriction(&(garbage->threeClient));
      //free(garbage);
      //printf("Pegando o melhor\n");
    }
    else {
      //garbage = aux;
      aux = aux->next;
      //freeThreeClientRestriction(&(garbage->threeClient));
      //free(garbage);
      //printf("Excluindo o pior\n");
    }
    //printf("Cai no limbo...\n");
  }

  (*threeClientR)->violationDegree *= -1;

  //free(*TCrList);
  //TCrList = NULL;
}

void convertTCRestrictionToGurobiCut(int m, 
	                             int n,
				     threeClientRestriction **threeClientR,
		                     int     *cutlen,
				     int    **cutind,
				     double **cutval)
{

  int i,j,k,u;
  int L0,L1,L2,L3;
  int J1,J2,J3;
  int *S12,*S13,*S23,*S123;
  Stype *aux;

  if (*threeClientR != NULL){
    

    /* initialization of variables */
    J1 = (*threeClientR)->J[0];
    J2 = (*threeClientR)->J[1];
    J3 = (*threeClientR)->J[2];
	  
    L0 = (*threeClientR)->Slength[0];
    L1 = (*threeClientR)->Slength[1];
    L2 = (*threeClientR)->Slength[2];
    L3 = (*threeClientR)->Slength[3];

    if(L0!=0)
      MEMALLOC(S12,L0);
    if(L1!=0)
      MEMALLOC(S13,L1);
    if(L2!=0)
      MEMALLOC(S23,L2);
    if(L3!=0)
      MEMALLOC(S123,L3);

    i=0;
    aux = (*threeClientR)->S12;
    while (aux != NULL){
      S12[i] = aux->index;
      aux = aux->next;
      i+=1;
    }
    i=0;
    aux = (*threeClientR)->S13;
    while (aux != NULL){
      S13[i] = aux->index;
      aux = aux->next;
      i+=1;
    }
    i=0;
    aux = (*threeClientR)->S23;
    while (aux != NULL){
      S23[i] = aux->index;
      aux = aux->next;
      i+=1;
    }
    i=0;
    aux = (*threeClientR)->S123;
    while (aux != NULL){
      S123[i] = aux->index;
      aux = aux->next;
      i+=1;
    }

    /* cutlen computation */
    *cutlen = 3*L0 + 3*L1 + 3*L2 + 4*L3;

    /* cutind and cutval computation */
    MEMALLOC(*cutind,*cutlen);
    MEMALLOC(*cutval,*cutlen);
   

    u = 0;
    for(k=0;k<u+L0 && L0!=0;k+=1){
      i = S12[k-u];
      j = J1;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }
    u = L0;
    for(;k<u+L0 && L0!=0;k+=1){
      i = S12[k-u];
      j = J2;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }
    u = 2*L0;
    for(;k<u+L1 && L1!=0;k+=1){
      i = S13[k-u];
      j = J1;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }
    u = 2*L0+L1;
    for(;k<u+L1 && L1!=0;k+=1){
      i = S13[k-u];
      j = J3;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }

    u = 2*L0+2*L1;
    for(;k<u+L2 && L2!=0;k+=1){
      i = S23[k-u];
      j = J2;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }
    u = 2*L0+2*L1+L2;
    for(;k<u+L2 && L2!=0;k+=1){
      i = S23[k-u];
      j = J3;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 2;
    }

    u = 2*L0+2*L1+2*L2;
    for(;k<u+L3 && L3!=0;k+=1){
      i = S123[k-u];
      j = J1;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 1;
    }
    u = 2*L0+2*L1+2*L2+L3;
    for(;k<u+L3 && L3!=0;k+=1){
      i = S123[k-u];
      j = J2;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 1;
    }
    u = 2*L0+2*L1+2*L2+2*L3;
    for(;k<u+L3 && L3!=0;k+=1){
      i = S123[k-u];
      j = J3;
      (*cutind)[k] = i + j*m; 
      (*cutval)[k] = 1;
    }

    u = 2*L0+2*L1+2*L2+3*L3;
    for(;k<u+L0 && L0!=0;k+=1){
      i = S12[k-u];
      (*cutind)[k] = m*n + i; 
      (*cutval)[k] = -2;
    }
    u = 3*L0+2*L1+2*L2+3*L3;
    for(;k<u+L1 && L1!=0;k+=1){
      i = S13[k-u];
      (*cutind)[k] = m*n + i; 
      (*cutval)[k] = -2;
    }
    u = 3*L0+3*L1+2*L2+3*L3;
    for(;k<u+L2 && L2!=0;k+=1){
      i = S23[k-u];
      (*cutind)[k] = m*n + i; 
      (*cutval)[k] = -2;
    }
    u = 3*L0+3*L1+3*L2+3*L3;
    for(;k<u+L3 && L3!=0;k+=1){
      i = S123[k-u];
      (*cutind)[k] = m*n + i; 
      (*cutval)[k] = -1;
    }
  }
}

