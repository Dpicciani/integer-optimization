
for client = 1:currentFormulation.n
  
  nonClosedDemands = find(problemReduction.fixedDemands(:, client) ~= 0);
  
  if length(nonClosedDemands) == 1
    problemReduction.fixedDemands(nonClosedDemands, client) = 1;
    problemReduction.openFacilities =...
      union(problemReduction.openFacilities, nonClosedDemands);
  end  
end