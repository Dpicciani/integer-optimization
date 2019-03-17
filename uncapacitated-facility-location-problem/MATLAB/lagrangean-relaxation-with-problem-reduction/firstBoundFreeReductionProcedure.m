
calculateDeltaForEachClient;

for client = 1:currentFormulation.n
  
  getVariablesToEliminate;
  
  eliminateFoundVariablesForCurrentClient; 
end

updateProblemReductionDegree;

clear delta variablesToEliminate;