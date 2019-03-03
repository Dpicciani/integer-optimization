% this script runs an optimization algorithm based on Beasley's 1993-paper
% "Lagrangean heuristics for location problems". A lagrangean
% relaxation with subgradient updates are applied on a Simple Plant 
% Location Problem, also known as Uncapacited Warehouse Location Problem.
%
% In addition, it is applied an aggressive reduction for the current
% problem guided by the Letchford's paper "An aggressive reduction scheme
% for the simple plant location problem". 
%
% LLBP: Lagrangean Lower Bound Problem

clear all; close all; clc;

initializeFormulationAndParameters;

tic
boundFreeProblemReduction;

while ~isStopConditionAchieved(LLBP,incumbent)

    solveLagrangeanLowerBoundProblemByInspection;
    tryToUpdateTheMaximumLBFoundAndResetsubgradientIterations;
  
    tryToGenerateAFeasibleSolutionFromTheLagrangeanSolution;
    calculateAndUpdateTheUpperBoundFound;
    
    adjustStepLengthParameterBasedOnSubgradientIterations;
    
    boundDependentProblemReduction;
    
    calculateTheSubGradients;
    defineAStepSizeForSubGradientsOptimization;
    updateTheLagrangeanMultipliersAccordingly;
    
    updateTotalNumberOfSubgradientIterations;
    
    displaySomeResults;
end
toc

problemReduction.degree