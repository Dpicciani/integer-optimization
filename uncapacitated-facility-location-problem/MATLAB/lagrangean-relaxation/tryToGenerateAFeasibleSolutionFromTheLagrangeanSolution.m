currentSolution.feasible.y = currentSolution.y;
currentSolution.feasible.x = zeros(currentFormulation.m,...
                                     currentFormulation.n);

allocateClosestClientsToOpenFacilities;