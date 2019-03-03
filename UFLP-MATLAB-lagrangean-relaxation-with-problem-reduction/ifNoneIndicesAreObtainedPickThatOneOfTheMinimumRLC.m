
currentSolution.LLBPSolved = 'yes';
if isempty(negativeRLCIndices)
  [~, negativeRLCIndices] = min(LLBP.rlc);
  currentSolution.LLBPSolved = 'no';
end