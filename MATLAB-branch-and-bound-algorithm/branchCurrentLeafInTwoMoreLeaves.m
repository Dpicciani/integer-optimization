solutionCoordinate = nonIntegerPositionIndex(1);

[ x_downer, x_upper] =...
floorAndCeil(currentLeaf.solution, solutionCoordinate);

createFirstLeafWithAdditionalRestrictions;
createSecondLeafWithAdditionalRestrictions;

listOfActiveLeaves = [listOfActiveLeaves, firstLeaf, secondLeaf];

clear solutionCoordinate x_downer x_upper firstLeaf secondLeaf;
