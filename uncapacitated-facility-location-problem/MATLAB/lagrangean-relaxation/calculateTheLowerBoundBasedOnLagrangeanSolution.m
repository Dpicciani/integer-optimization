if ~isempty( find(currentSolution.y ~= 0, 1) )
  currentSolution.lowerBound = LLBP.rlc'*currentSolution.y + sum(LLBP.s);
end