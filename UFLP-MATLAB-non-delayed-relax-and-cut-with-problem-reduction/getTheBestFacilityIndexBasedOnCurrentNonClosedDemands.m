
getPossibleSupplyingCostsFromCurrentClientNonClosedDemands;

[~, bestFacilityIndex] = min( possibleSupplyingCosts );

bestFacilityIndex =...
  openFacilitiesIndices(nonClosedDemands(bestFacilityIndex));