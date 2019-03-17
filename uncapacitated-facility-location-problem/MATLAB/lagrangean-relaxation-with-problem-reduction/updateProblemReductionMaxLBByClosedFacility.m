
for facilityIndex = 1:currentFormulation.m
  if currentSolution.y(facilityIndex) == 0
    
    updatePRMaxLBByClosedFacilityComparingWithCurrentSolutionLB;
    
  %elseif ~ismember(facilityIndex, problemReduction.openFacilities)
  else 
    
    updateProblemReductionMaxLBFoundByClosingACurrentOpenFacility;

  end
end