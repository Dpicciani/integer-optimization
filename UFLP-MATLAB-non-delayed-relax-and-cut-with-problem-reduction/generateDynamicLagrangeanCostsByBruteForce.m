dynamicLC = zeros(currentFormulation.m, currentFormulation.n);

numberOfViolatedRestrictions = length(previouslyViolatedActiveSet);

for violatedRestrictionIndex = 1:numberOfViolatedRestrictions
  
 restriction = previouslyViolatedActiveSet{violatedRestrictionIndex};
  
 for facilityIndex = 1:currentFormulation.m
  for clientIndex = 1:currentFormulation.n
      
   dynamicLC(facilityIndex,clientIndex) = ...
   dynamicLC(facilityIndex,clientIndex) + ...
     LLBP.dS(violatedRestrictionIndex)*LLBP.dX( facilityIndex,...
                                                clientIndex,...
                                                currentSolution,...
                                                restriction);
  end
 end
end