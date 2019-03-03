
%[sortedRLC, IndexesOfChosenRLC] = sort(LLBP.rlc); 
%
%IndexesOfChosenRLC = IndexesOfChosenRLC...
%  (1:currentFormulation.desiredNumberOfOpenFacilities);

IndexesOfChosenRLC = find(LLBP.rlc <= 0);

currentSolution.LLBPSolved = 'yes';
if isempty(IndexesOfChosenRLC)
  %IndexesOfChosenRLC = randi(currentFormulation.m);
  [~, IndexesOfChosenRLC] = min(LLBP.rlc);
  
  currentSolution.LLBPSolved = 'no';
end

% RLC: reduced lagrangean costs