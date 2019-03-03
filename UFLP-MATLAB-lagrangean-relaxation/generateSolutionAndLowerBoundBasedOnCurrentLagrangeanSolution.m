currentSolution.x = zeros(currentFormulation.m, currentFormulation.n);

for clientIndex = 1:currentFormulation.n % LC: lagrangean costs
  openFacilitiesIndexes = find(currentSolution.y == 1);
  
  positiveLCIndexes = find(LLBP.lc( openFacilitiesIndexes,...
                                    clientIndex) > 0);
  
  closeFacilitiesIndexes = openFacilitiesIndexes(positiveLCIndexes);
                                  
  openFacilitiesForCurrentClient = currentSolution.y;
  openFacilitiesForCurrentClient(closeFacilitiesIndexes) = 0;
                       
  currentSolution.x(openFacilitiesForCurrentClient == 1, clientIndex) = 1;
end

calculateTheLowerBoundBasedOnLagrangeanSolution;