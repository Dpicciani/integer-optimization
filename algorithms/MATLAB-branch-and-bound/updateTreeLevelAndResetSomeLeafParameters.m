function leaf = updateTreeLevelAndResetSomeLeafParameters(leaf)

leaf.treeLevel = leaf.treeLevel + 1;
leaf.upperBound = Inf;
leaf.lowerBound = -Inf;
leaf.solution = [];
leaf.linProgFlag = [];
end