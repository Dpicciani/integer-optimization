
for facilityIndex = 1:currentFormulation.m
  if currentSolution.y(facilityIndex) == 1
    
    updatePRMaxLBByOpenFacilityComparingWithCurrentSolutionLB;
  
  else % if facility is closed
  
    updateProblemReductionMaxLBFoundByOpeningACurrentClosedFacility;
  
  end
end