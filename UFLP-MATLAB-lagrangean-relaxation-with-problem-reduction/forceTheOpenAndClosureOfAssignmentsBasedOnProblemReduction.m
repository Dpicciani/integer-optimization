
negativeLCAssignmentsIndices =...
  setdiff(negativeLCAssignmentsIndices,...
          find(problemReduction.fixedDemands(facilityIndex,:) == 0));
  
negativeLCAssignmentsIndices =...
  union(negativeLCAssignmentsIndices,...
        find(problemReduction.fixedDemands(facilityIndex,:) == 1));