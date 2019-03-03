function [booleanResult, nonIntegerPositionIndex] = isIntegerSolution( solution )

roundedSolution = floor(solution);

nonIntegerPositionIndex = find( roundedSolution ~= solution );

if isempty(nonIntegerPositionIndex) 
  booleanResult = true;
else
  booleanResult = false;
end

end