LLBP.lc = currentFormulation.c - repmat(LLBP.s, [currentFormulation.m,1]);

% note here that the function repmat uses much more memory than an
% implementation of the same calculation using for-loop, but it is faster.