function degree = getProblemReductionDegree( problemReduction )

  [m,n] = size(problemReduction.fixedDemands);

  degree = 100 - sum(-sum(problemReduction.fixedDemands))*100/(m*n);

end