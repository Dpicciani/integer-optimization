% second restriction
secondLeaf = currentLeaf;

secondLeaf = updateTreeLevelAndResetSomeLeafParameters(secondLeaf);

secondLeaf.additionalRestrictions.A =...
 [ secondLeaf.additionalRestrictions.A ;
 -binaryVector(originalFormulation.solutionDimension, solutionCoordinate)];

secondLeaf.additionalRestrictions.b =...
 [secondLeaf.additionalRestrictions.b ; -x_upper];