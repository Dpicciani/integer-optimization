if ~isempty(currentLeaf.prunedBy)
  pruneState = ['| pruned by ' currentLeaf.prunedBy];
else
  pruneState = '';
end

fprintf('x: [%4.2f %4.2f] | z_ub = %5.2f | z_lb = %5.2f', incumbent.solution(1), incumbent.solution(2), incumbent.upperBound, incumbent.lowerBound );
fprintf(' || current Leaf - treeLevel: %3d | solution: [%4.2f %4.2f] | upperBound: %5.2f | lowerBound: %5.2f %s\n', currentLeaf.treeLevel, currentLeaf.solution(1), currentLeaf.solution(2), currentLeaf.upperBound, currentLeaf.lowerBound, pruneState );