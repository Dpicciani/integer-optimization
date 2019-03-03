% first restriction
% solution(solutionCoordinate) <= x_downer
firstLeaf = currentLeaf;

firstLeaf = updateTreeLevelAndResetSomeLeafParameters(firstLeaf);

if x_downer == 0
 firstLeaf.additionalRestrictions.Aeq =...
  [ firstLeaf.additionalRestrictions.Aeq ;
  binaryVector(originalFormulation.solutionDimension, solutionCoordinate)];

 firstLeaf.additionalRestrictions.beq =...
  [ firstLeaf.additionalRestrictions.beq ; 0 ];
else
 firstLeaf.additionalRestrictions.A =...
  [ firstLeaf.additionalRestrictions.A ;
  binaryVector(originalFormulation.solutionDimension, solutionCoordinate)];

 firstLeaf.additionalRestrictions.b =...
  [ firstLeaf.additionalRestrictions.b ; x_downer ];
end


