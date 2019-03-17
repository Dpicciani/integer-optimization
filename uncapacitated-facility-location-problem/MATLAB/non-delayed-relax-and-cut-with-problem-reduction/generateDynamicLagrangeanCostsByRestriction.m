dynamicLC = zeros(currentFormulation.m, currentFormulation.n);

numberOfViolatedRestrictions = length(previouslyViolatedActiveSet);

for violatedRestrictionIndex = 1:numberOfViolatedRestrictions
  
 restriction = previouslyViolatedActiveSet{violatedRestrictionIndex};
  
 for facilityIndex = [restriction.S12 restriction.S13 restriction.S23 restriction.S123]
  for clientIndex = restriction.J
      
   dynamicLC(facilityIndex,clientIndex) = ...
   dynamicLC(facilityIndex,clientIndex) + ...
     LLBP.dS(violatedRestrictionIndex)*LLBP.dX( facilityIndex,...
                                                clientIndex,...
                                                currentSolution,...
                                                restriction);
  end
 end
end