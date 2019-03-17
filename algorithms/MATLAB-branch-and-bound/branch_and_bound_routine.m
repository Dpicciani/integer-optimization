% this script runs a branch-and-bound algorithm to solve a predefined 
% maximization integer programming problem.

clc; close all; clear all; 

initializeFormulationAndVariablesAndParameters;

while ~isempty(listOfActiveLeaves)
  
  selectALeaf;
  
  updateOriginalFormulationWithAdditionalRestrictionsFromLeaf;
  
  solveLeafProblemByLinearRelaxation;
  
  updateUpperBoundAtFirstLeaf;
  
  pruneOrBranchCurrentLeaf;
  
  printIncumbentSolution;
  
end

