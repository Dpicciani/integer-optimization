function I =...
  createAIndicesSetAssociatedToNonFixedVariablesFromEachClient...
  ( currentFormulation, problemReduction )

  for j = 1:currentFormulation.n
    I{j} = find(problemReduction.fixedDemands(:,j) < 0)';
  end
end