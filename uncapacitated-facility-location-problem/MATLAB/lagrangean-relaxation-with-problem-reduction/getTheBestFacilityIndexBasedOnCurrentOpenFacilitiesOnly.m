
getPossibleSupplyingCostsFromOpenFacilities;

[~, bestFacilityIndex] = min( possibleSupplyingCosts );

bestFacilityIndex = openFacilitiesIndices(bestFacilityIndex);