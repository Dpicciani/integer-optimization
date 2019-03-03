currentSolution.y = zeros(currentFormulation.m, 1);
currentSolution.y(negativeRLCIndices,1) = 1;

clear negativeRLCIndices;