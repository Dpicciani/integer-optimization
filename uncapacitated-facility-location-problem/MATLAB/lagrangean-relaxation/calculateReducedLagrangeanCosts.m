for facilityIndex = 1:currentFormulation.m
  negativeLagrangeanCosts = LLBP.lc( facilityIndex,...
                                     (LLBP.lc(facilityIndex,:) < 0) );
                                   
  LLBP.rlc(facilityIndex,1) = currentFormulation.F(facilityIndex,1) +...
                              sum( negativeLagrangeanCosts );
end

clear negativeLagrangeanCosts;