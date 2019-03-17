openFacilitiesIndices = find(currentSolution.feasible.y == 1);

for client = 1:currentFormulation.n  
  getNonClosedDemandsFromCurrentClientProblemReduction;
  
  if ~isempty(nonClosedDemands)
    getTheBestFacilityIndexBasedOnCurrentNonClosedDemands;
  else
    getTheBestFacilityIndexBasedOnCurrentOpenFacilitiesOnly;
  end
   
  allocateCurrentClientToTheBestFacilityFound;
end

clear openFacilitiesIndices nonClosedDemands bestFacilityIndex client...
      possibleSupplyingCosts;