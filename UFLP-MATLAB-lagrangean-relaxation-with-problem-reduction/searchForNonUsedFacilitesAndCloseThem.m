
for facility = 1:currentFormulation.m
  
  nonClosedDemands = find(problemReduction.fixedDemands(facility,:) ~= 0);
  
  if isempty(nonClosedDemands)
    problemReduction.closedFacilities =...
      union(problemReduction.closedFacilities, facility);
  end  
end