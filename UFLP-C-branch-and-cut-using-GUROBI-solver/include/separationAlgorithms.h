#ifndef _SEPARATION_ALGORITHMS_
#define _SEPARATION_ALGORITHMS_

#include "uflpBranchAndCut.h"


/* 3-client inequalities */
typedef struct Xfractional {

  int index;

  struct Xfractional *next;

} Xfractional;

typedef struct Jfractional {

  int index;
  int degree;

  struct Jfractional *next;
  Xfractional *Xfrac;

} Jfractional;

typedef struct Stype {

  int index;
  struct Stype *next;

} Stype;

typedef struct {

  Stype *S12;
  Stype *S13;
  Stype *S23;
  Stype *S123;
  int    J[3];
  double violationDegree;
  int    Slength[4];
  int    token1;
  int    token2;
  int    token3;

} threeClientRestriction;

typedef struct threeClientRestrictionList {

  int id;
  threeClientRestriction      *threeClient;
  struct threeClientRestrictionList *next;

} threeClientRestrictionList;



void threeClientSeparation(int m, 
		           int n, 
			   solution **sol, 
			   threeClientRestrictionList **TCrList,
			   int *TCrLength);
void computeJfAndEf(int m, 
		    int n,
		    solution **sol, 
		    Jfractional **Jbase, 
		    int *fractionaryClients);

void JfracMemoryAllocation(Jfractional **Jf);

void XfracMemoryAllocation(Xfractional **Xf);

void threeClientJSetting
      (threeClientRestriction **threeClientR, int j1, int j2, int j3);

void getIndicesOfXfWithoutDuplicatedElements
      (Jfractional *J1, Jfractional *J2, Jfractional *J3 );

void getIndicesOfFractionaryXBasedOnCurrentT
      (int **IXf, Jfractional *J1, Jfractional *J2, Jfractional *J3 );

void getAllXfValuesFromJfractional(int *i, 
		                   int **IXf, 
				   struct Jfractional *J);

int  degreeSum(struct Jfractional *J1, 
	       struct Jfractional *J2, 
       	       struct Jfractional *J3);

void cleanDuplicatedElements(int **IXfWD, int **IXf, int size, int *sizeOut);

void updatePointersForPermutationOfTripleClients(int *stop, 
		                                  Jfractional **J1, 
						  Jfractional **J2, 
						  Jfractional **J2_, 
						  Jfractional **J3, 
						  Jfractional **J3_);

void printJfEf(Jfractional *Jfbase);

void separationTest();

void threeClientMemoryAllocation(threeClientRestriction **threeClientR);

void addIndexToSetS(Stype **S, int index);

void createTokens(threeClientRestriction **threeClientR);

void createToken1(threeClientRestriction **threeClientR);

void addSumOfIndicesToToken(int *indicesSum, Stype *S);

void createToken2(threeClientRestriction **threeClientR);

void createToken3(threeClientRestriction **threeClientR);

void analyzeAcceptanceAndUpdateRestrictionList
     (threeClientRestriction **threeClientR, 
      threeClientRestrictionList **TCrList,
      int *TCrLength);

void freeStypeMemory(Stype **S);

void freeThreeClientRestriction(threeClientRestriction **TCR);

void getTheMostViolatedRestrictionAndFreeTheOthers
     (threeClientRestriction **threeClientR, threeClientRestrictionList **TCrList);

void convertTCRestrictionToGurobiCut(int m,
				     int n,
		                     threeClientRestriction **threeClientR, 
		                     int     *cutLen,
				     int    **cutind,
				     double **cutval);

#endif
