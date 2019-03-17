if ~isempty( find(currentSolution.y ~= 0, 1) )
  currentSolution.lowerBound = LLBP.rlc'*currentSolution.y +...
    sum(LLBP.s) + sum(LLBP.dS)*LLBP.dI();
end