for facilityIndex = 1:currentFormulation.m
  getNegativeLCAssignementsIndices;
  
  forceTheOpenAndClosureOfAssignmentsBasedOnProblemReduction;
  
  getNegativeLagrangeanCosts;
                            
  calculateTheReducedLagrangeanCostForCurrentFacility;
  
  insertionOfYTermsFromDynamicRestrictions;
end

clear facilityIndex negativeLagrangeanCosts negativeLCAssignmentsIndices;