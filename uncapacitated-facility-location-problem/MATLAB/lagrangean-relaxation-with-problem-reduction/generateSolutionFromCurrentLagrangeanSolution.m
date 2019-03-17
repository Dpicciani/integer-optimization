
currentSolution.x = zeros(currentFormulation.m, currentFormulation.n);

for clientIndex = 1:currentFormulation.n % LC: lagrangean costs
  
  tryToFixASupplyingVariableForCurrentClient;
  
  getOpenFacilitiesIndices;
  
  selectOpenFacilitiesForCurrentClientWithNonPositiveLC;
 
  openDemandsInCurrentSolutionForCurrentClient;  
  
  applyProblemReductionToCurrentSolution;
end