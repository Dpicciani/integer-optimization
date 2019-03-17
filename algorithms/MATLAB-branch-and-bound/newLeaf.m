function leaf = newLeaf()

leaf.treeLevel = 0;
leaf.additionalRestrictions.A = [];
leaf.additionalRestrictions.b = [];
leaf.additionalRestrictions.Aeq = [];
leaf.additionalRestrictions.beq = [];
leaf.upperBound = Inf;
leaf.lowerBound = -Inf;
leaf.solution = [];
leaf.linProgFlag = [];
end