if currentLeaf.linProgFlag < 0
  % Leaf pruned by infeasibility
  currentLeaf.prunedBy = 'infeasibility';
  
elseif currentLeaf.upperBound < incumbent.lowerBound
  % Leaf pruned by bound
  currentLeaf.prunedBy = 'bound';
else
  [ integerFlag, nonIntegerPositionIndex] =...
  isIntegerSolution(currentLeaf.solution);
  
  if integerFlag
    % Leaf pruned by optimality
    currentLeaf.lowerBound = currentLeaf.upperBound;
    currentLeaf.prunedBy = 'optimality';
    
    incumbent.lowerBound = currentLeaf.upperBound;
    incumbent.solution = currentLeaf.solution;
    
  else
    branchCurrentLeafInTwoMoreLeaves;
  end
  
  clear integerFlag nonIntegerPositionIndex;
end