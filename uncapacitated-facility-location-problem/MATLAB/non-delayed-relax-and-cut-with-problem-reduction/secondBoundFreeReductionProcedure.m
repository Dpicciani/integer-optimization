
for client = 1:currentFormulation.n
 
  getFacilitiesIndicesOfNonEliminatedXVariablesForEachClient;
  
  sortTheNonFixedFacilitiesInNonDecreasingCosts;
  
  searchForTheMaximumSetOfVariablesIndicesToBeEliminated;
  
  updateFixedDemands;
end

updateProblemReductionDegree;

clear I_ I client ISorted;