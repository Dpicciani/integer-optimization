if currentSolution.upperBound < incumbent.minimumUpperBoundFound
  incumbent.minimumUpperBoundFound = currentSolution.upperBound;
  incumbent.feasibleSolution = currentSolution.feasible;
end