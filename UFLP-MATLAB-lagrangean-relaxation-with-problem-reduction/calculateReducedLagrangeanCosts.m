for facilityIndex = 1:currentFormulation.m
  getNegativeLCAssignementsIndices;
  
  forceTheOpenAndClosureOfAssignmentsBasedOnProblemReduction;
  
  getNegativeLagrangeanCosts;
                            
  calculateTheReducedLagrangeanCostForCurrentFacility;
end

clear facilityIndex negativeLagrangeanCosts negativeLCAssignmentsIndices;