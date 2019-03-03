if currentSolution.lowerBound > incumbent.maximumLowerBoundFound
  incumbent.maximumLowerBoundFound = currentSolution.lowerBound;
  LLBP.subgradientIterations = 0;
else
  LLBP.subgradientIterations = LLBP.subgradientIterations + 1;
end