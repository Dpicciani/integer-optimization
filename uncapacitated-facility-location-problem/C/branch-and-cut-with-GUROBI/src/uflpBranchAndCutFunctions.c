#include "uflpBranchAndCut.h"
#include "separationAlgorithms.h"
#include "errorHandling.h"
#include "gurobi_c.h"
#include <stdlib.h>
#include <string.h>

void initializeFormulationAndVariablesAndParameters(uflpModel **model)
{
  int m, n;
  FILE *databaseFile;

  openDatabaseFileForReading(&databaseFile);

  readingProblemDimensions(&databaseFile,&m,&n);

  modelMemoryAllocationAndParametersSetting(model, m, n);

  readingFacilityCosts(model, &databaseFile);

  readingDemandCosts(model, &databaseFile);  

  generateLPfileWithOriginalFormulation(model);

  free((*model)->F);
  free((*model)->C);
}

void openDatabaseFileForReading(FILE **databaseFile)
{
  (*databaseFile) = fopen(_INPUT_FILEPATH_, "r");
  if (!(*databaseFile)){
    printf(_OPEN_FILE_ERROR_MESSAGE_);
    exit(_OPEN_FILE_ERROR_);
  }
}

void readingProblemDimensions(FILE **databaseFile, int *m, int *n)
{
  fscanf((*databaseFile), "%d", m);
  fscanf((*databaseFile), "%d", n);
}

void modelMemoryAllocationAndParametersSetting(uflpModel **model, int m, int n)
{
  int i;

  /* Memory Allocation */
  MEMALLOC(*model, 1);
  MEMALLOC((*model)->F,m);
  MEMALLOC((*model)->C,m);
  for( i = 0; i < m; i += 1){
    MEMALLOC((*model)->C[i],n);
  }
  MEMALLOC((*model)->sol,1);
  MEMALLOC((*model)->sol->x,m);
  for( i = 0; i < m; i += 1){
    MEMALLOC((*model)->sol->x[i],n);
  }
  MEMALLOC((*model)->sol->y,m);
  
  /* Model settings */
  (*model)->m = m;
  (*model)->n = n;
  (*model)->minOrMax = _PROBLEM_TYPE_;
}

void readingFacilityCosts(uflpModel **model, FILE **databaseFile)
{
  int i;

  for(i = 0; i < (*model)->m; i+=1)
    fscanf((*databaseFile), "%*d %lf", &((*model)->F[i]));
}

void readingDemandCosts(uflpModel **model, FILE **databaseFile)
{
  int i,j;

  for (j = 0; j < (*model)->n; j += 1){
    for (i = 0; i < (*model)->m; i += 1)
      if (i==0)
        fscanf((*databaseFile), "%*d %lf ", &((*model)->C[i][j]));
      else
        fscanf((*databaseFile), "%lf ", &((*model)->C[i][j]));
    }
}

void generateLPfileWithOriginalFormulation(uflpModel **model)
{
  FILE *lpFile;
  int i,j, restrictionID;
  int lineLength;
  char string[_LP_STRING_BUFFER_LENGTH_];

  openLPfileForWriting(&lpFile);

  /* Problem Sense */
  if ((*model)->minOrMax == _MIN_)
    fprintf(lpFile,"MINIMIZE\n");
  else
    fprintf(lpFile,"MAXIMIZE\n");
 
  /* Objective Function */
  for(j = 1; j <= (*model)->n; j += 1){
    for(i = 1; i <= (*model)->m; i += 1){
      if (i==1 && j==1){
        sprintf(string, " obj: %.4lf x%03d%03d", (*model)->C[i-1][j-1], i, j);
        fprintf(lpFile, "%s", string);
	lineLength = strlen(string);
      }
      else{
	sprintf(string, " + %.4lf x%03d%03d", (*model)->C[i-1][j-1], i, j);
	lineLength += strlen(string);
	if (lineLength > _LP_LINE_LIMIT_){
	  fprintf(lpFile, "\n");
          lineLength = strlen(string);	  
	}
        fprintf(lpFile, "%s", string);
      }
    }
    fprintf(lpFile, "\n");
    lineLength = 0;
  }

  for(i = 1; i <= (*model)->m; i += 1){
    sprintf(string, " + %.4lf y%03d", (*model)->F[i-1], i);
    lineLength += strlen(string);
    if (lineLength > _LP_LINE_LIMIT_){
      fprintf(lpFile, "\n");
      lineLength = strlen(string);	  
    }
    fprintf(lpFile, "%s", string); 
  }

  /* Constraints */
  fprintf(lpFile, "\nSubject to\n");

  //sum_{i \in M} x_{ij} = 1, \forall j \in N
  lineLength = 0;
  restrictionID = 0;
  for(j = 1; j <= (*model)->n; j += 1){
    for (i = 1; i <= (*model)->m; i += 1){
      if (i==1){
        sprintf(string, "c%d: x%03d%03d", restrictionID, i, j);
	fprintf(lpFile, "%s", string);
	restrictionID += 1;
      }
      else{
        sprintf(string, " + x%03d%03d", i, j);
	lineLength += strlen(string);
	if (lineLength > _LP_LINE_LIMIT_){
          fprintf(lpFile, "\n");
          lineLength = strlen(string);	  
        }
	fprintf(lpFile, "%s", string);
      }
    }
    sprintf(string, " = 1\n");
    lineLength += strlen(string);
    if (lineLength > _LP_LINE_LIMIT_){
      fprintf(lpFile, "\n");
      lineLength = strlen(string);	  
    }
    fprintf(lpFile, "%s", string);
    
    lineLength = 0;
  }

  // x_{ij} - y_{i} <= 0, \forall i \in M, \forall j \in N
  for(i = 1; i <= (*model)->m; i += 1){
    for(j = 1; j <= (*model)->n; j += 1){
      fprintf(lpFile, "c%d: x%03d%03d - y%03d <= 0\n", restrictionID,i,j,i);
      restrictionID += 1;
    }
  }

  /* Bounds */
  //fprintf(lpFile, "Bounds\n");
  /*
  for(i = 1; i <= (*model)->m; i += 1)
    for(j = 1; j <= (*model)->n; j += 1)
      fprintf(lpFile, " 0 <= x%03d%03d <= 1\n", i,j);
  
  for(i = 1; i <= (*model)->m; i += 1)
    fprintf(lpFile, " y%03d <= 1\n", i);
  */

  /* Binary variables */
  lineLength = 0;
  fprintf(lpFile, "Binary\n");
  for(i = 1; i <= (*model)->m; i += 1)
    for(j = 1; j <= (*model)->n; j += 1){
      sprintf(string, " x%03d%03d", i,j);
      lineLength += strlen(string);
      if (lineLength > _LP_LINE_LIMIT_){
        fprintf(lpFile, "\n");
        lineLength = strlen(string);	  
      }
      fprintf(lpFile, "%s", string);
    }

  for(i = 1; i <= (*model)->m; i += 1){
    sprintf(string, " y%03d", i);
    lineLength += strlen(string);
    if (lineLength > _LP_LINE_LIMIT_){
      fprintf(lpFile, "\n");
      lineLength = strlen(string);	  
    }
    fprintf(lpFile, "%s", string);
  }
  /* END OF FILE */
  fprintf(lpFile, "\nEND");

  fclose(lpFile);
}

void openLPfileForWriting(FILE **lpFile)
{
  (*lpFile) = fopen(_LP_FILEPATH_, "w");
  if (!(*lpFile)){
    printf(_OPEN_FILE_ERROR_MESSAGE_);
    exit(_OPEN_FILE_ERROR_);
  }
}

void gurobiSolver(uflpModel **model)
{
  int m = (*model)->m;
  int n = (*model)->n;
  int solDim = m*n + m;

  GRBenv   *env   = NULL;
  GRBmodel *gModel = NULL;
  int 	   error  = 0, i, j;
  double   sol[solDim];
  int      optimstatus;
  double   objval;
  struct callback_data mydata;

  /* initializing callback data */
  mydata.m = m;
  mydata.n = n;
  mydata.TCR = NULL;

  /* Create environment */
  error = GRBloadenv( &env, _LOG_FILEPATH_);
  if (error) errorMsg(1231, "GRBloadenv", env);

  error = GRBreadmodel(env, _LP_FILEPATH_, &gModel);
  if (error) errorMsg(1232, "GRBreadmodel", env);
  
  /* Parameters and Attributes Setting*/

  //error = GRBsetintattr(gModel,GRB_INT_ATTR_BRANCHPRIORITY, 1); //only for MIP
  //error = GRBsetintattr(gModel,GRB_INT_ATTR_VARHINTPRI, 1); //only for MIP
  //error = GRBsetdblattr(gModel,GRB_DBL_ATTR_VARHINTVAL, 1); //only for MIP
  
  error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_MIPFOCUS, 0); //only for MIP
  error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_THREADS, 1);
  error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_CUTS, 0); //only for MIP
  error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_PRESOLVE, 0);
  


  error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_PRECRUSH, 1);
  
  
  
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_BRANCHDIR, 0); //only for MIP
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_DISPLAYINTERVAL, 0);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_VARBRANCH, 0);
  
  //error = GRBsetdblparam(GRBgetenv(gModel), GRB_DBL_PAR_TIMELIMIT, 150);
  //error = GRBsetdblparam(GRBgetenv(gModel), GRB_DBL_PAR_NODELIMIT, 5);
  //error = GRBsetdblparam(GRBgetenv(gModel), GRB_DBL_PAR_HEURISTICS, 0.05);


  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_TUNECRITERION, 0);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_TUNEJOBS, 1);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_TUNEOUTPUT, 1);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_TUNERESULTS, 10);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_DBL_PAR_TUNETIMELIMIT, -1);
  //error = GRBsetintparam(GRBgetenv(gModel),GRB_INT_PAR_TUNETRIALS, 10);

  //error = GRBtunemodel(gModel);
  
  if (error) errorMsg(12351, "Error setting parameters", env);

  //error = GRBsetstrparam(env,);

  /* Set CALLBACK function */
  error = GRBsetcallbackfunc(gModel, mycallback, (void *) &mydata);
  if (error) errorMsg(12341, "Error setting callback function", env);


  /* OPTIMIZE MODEL */
  error = GRBoptimize(gModel);
  if (error) errorMsg(1236, "GRBOptimize", env);


  /* Write model to the file 'mochila01.lp' */
  error = GRBwrite(gModel, _LP_OUTPUT_FILEPATH_);
  if (error) errorMsg(1237, "Creating .lp file.", env);


  /* Capture solution information */
  error = GRBgetintattr(gModel, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) errorMsg(1238, "Getting the status", env);
  error = GRBgetdblattr(gModel, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) errorMsg(1239, "Getting the value of objective function", env);
  error = GRBgetdblattrarray(gModel, GRB_DBL_ATTR_X, 0, solDim, sol);
  if (error) errorMsg(1240, "Getting the value of the solution" , env);


  /* Free model */
  GRBfreemodel(gModel);


  /* Free environment */
  GRBfreeenv(env);

  /* Recording solution */
  for (i = 0; i < m; i += 1)
    for (j = 0; j < n; j += 1)
      (*model)->sol->x[i][j] = sol[i + j*m];
  for (i = 0; i < m; i += 1)
    (*model)->sol->y[i] = sol[m*n +i];

  (*model)->sol->z = objval;
}

void errorMsg(const int er, const char *msg, GRBenv *env)
{
  printf("\nERROR: %s\n", GRBgeterrormsg(env));
  exit(er);
}

void sendResultsToAnOutputFile(uflpModel **model)
{
  FILE *outputFile;
  int i,j;

  openOutputFileForWriting(&outputFile);

  /* Objective function value */
  fprintf(outputFile,"%lf\n" ,(*model)->sol->z);

  /* X variable */
  for(i = 0; i < (*model)->m; i += 1){
    for(j = 0; j < (*model)->n; j += 1)
      fprintf(outputFile,"%d " ,(int) (*model)->sol->x[i][j]);
    fprintf(outputFile,"\n");
  }

  fprintf(outputFile, "\n");

  /* Y variable */
  for (i = 0; i < (*model)->m; i += 1)
    fprintf(outputFile,"%d " ,(int) (*model)->sol->y[i]);

  fclose(outputFile);
}

void openOutputFileForWriting(FILE **outputFile)
{
  (*outputFile) = fopen(_OUTPUT_FILEPATH_, "w");
  if (!(*outputFile)){
    printf(_OPEN_FILE_ERROR_MESSAGE_);
    exit(_OPEN_FILE_ERROR_);
  }
}

int __stdcall mycallback(GRBmodel *model,
		         void     *cbdata,
			 int       where,
			 void     *usrdata)
{
  struct callback_data *mydata = (struct callback_data *) usrdata;

  switch(where){
    case GRB_CB_POLLING:
    break;
    
    case GRB_CB_SIMPLEX:
    break;
    
    case GRB_CB_MIP:
    break;
    
    case GRB_CB_MIPSOL:
    break;
    
    case GRB_CB_MIPNODE: {
      int i,j,error;
      int m = mydata->m;
      int n = mydata->n;
      double gSol[m*n + m];
      int TCrLength = 0;
      threeClientRestriction *threeClientR;
      //int numberOfFracX;
      solution *sol;
      int      cutlen;
      int     *cutind;
      double  *cutval;
      int      status;

      GRBcbget(cbdata,where,GRB_CB_MIPNODE_STATUS,&status);

      if (status == GRB_OPTIMAL){
        MEMALLOC(sol,1);
        MEMALLOC(sol->x, m);
        for(i=0;i<m;i+=1){
          MEMALLOC(sol->x[i],n);
        }
        MEMALLOC(sol->y,m);

        GRBcbget(cbdata, where, GRB_CB_MIPNODE_REL, gSol);
      
        /* transforming solution for 3 client separation */
        for (i = 0; i < m; i += 1)
          for (j = 0; j < n; j += 1)
            sol->x[i][j] = gSol[i + j*m];
  
        for (i = 0; i < m; i += 1)
          sol->y[i] = gSol[m*n +i];

        threeClientSeparation(m,n,&sol,&(mydata->TCR), &TCrLength);

        //printf("Number of new violated restrictions found: %d\n\n",TCrLength);

        getTheMostViolatedRestrictionAndFreeTheOthers(&threeClientR, &(mydata->TCR));
     
        //printf("Violation : %lf", threeClientR->violationDegree);
 
        convertTCRestrictionToGurobiCut(m,
	  	                        n,
				        &threeClientR,
				        &cutlen, 
				        &cutind,
				        &cutval);      

     
        //printf("token = %d %d %d\n", threeClientR->token1,threeClientR->token2,threeClientR->token3);

        error = GRBcbcut(cbdata,cutlen,cutind,cutval,GRB_LESS_EQUAL,2);
        if (error){
          printf("\nGRBcbcut crashed.\n");
	  return 0;
        }

        TCrLength = 0;
        mydata->TCR = NULL;
      }
    }
    break;
    
    case GRB_CB_BARRIER:
      break;
    
    case GRB_CB_MESSAGE:
      break;
    
    default: ; 
  }

  return OK;
}
