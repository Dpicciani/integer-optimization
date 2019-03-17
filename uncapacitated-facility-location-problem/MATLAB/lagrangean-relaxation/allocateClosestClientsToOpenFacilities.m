openFacilitiesIndexes = find(currentSolution.feasible.y == 1);

for client = 1:currentFormulation.n
  possibleSupplyingCosts =...
    currentFormulation.c(openFacilitiesIndexes, client);
  
  [~, bestFacilityIndex] = min( possibleSupplyingCosts );
  bestFacilityIndex = openFacilitiesIndexes(bestFacilityIndex);
  
  currentSolution.feasible.x(bestFacilityIndex,client) = 1;
end