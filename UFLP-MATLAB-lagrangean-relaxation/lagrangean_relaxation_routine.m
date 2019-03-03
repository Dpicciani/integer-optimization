% this script runs an optimization algorithm based on Beasley's 1993-paper
% "Lagrangean heuristics for location problems". A lagrangean
% relaxation with subgradient updates are applied on a Simple Plant 
% Location Problem, also known as Uncapacited Warehouse Location Problem.
%
% LLBP: Lagrangean Lower Bound Problem

clear all; close all; clc;

initializeFormulationAndParameters;

tic
while ~isStopConditionAchieved(LLBP,incumbent)

    solveLagrangeanLowerBoundProblemByInspection;
    tryToUpdateTheMaximumLBFoundAndResetsubgradientIterations;
  
    tryToGenerateAFeasibleSolutionFromTheLagrangeanSolution;
    calculateAndUpdateTheUpperBoundFound;
    
    adjustStepLengthParameterBasedOnSubgradientIterations;
    
    calculateTheSubGradients;
    defineAStepSizeForSubGradientsOptimization;
    updateTheLagrangeanMultipliersAccordingly;
    
    updateTotalNumberOfSubgradientIterations;
    
    displaySomeResults;
end

toc