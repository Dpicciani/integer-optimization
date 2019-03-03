
positiveLCIndices = find(LLBP.lc( openFacilitiesIndices, clientIndex) > 0);
  
closeFacilitiesIndices = openFacilitiesIndices(positiveLCIndices);
                                  
openFacilitiesForCurrentClient = currentSolution.y;
openFacilitiesForCurrentClient(closeFacilitiesIndices) = 0;