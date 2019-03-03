
uniqueSupplying = find(problemReduction.fixedDemands(:,clientIndex)<0);
if length(uniqueSupplying) == 1
  problemReduction.fixedDemands(uniqueSupplying,clientIndex) = 1;
end