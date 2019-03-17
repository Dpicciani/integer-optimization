
problemReduction.fixedDemands = firstFixedDemands;

distinctEntries = find(firstFixedDemands ~= secondFixedDemands);

problemReduction.fixedDemands(distinctEntries) = 0;

clear firstFixedDemands secondFixedDemands distinctEntries;