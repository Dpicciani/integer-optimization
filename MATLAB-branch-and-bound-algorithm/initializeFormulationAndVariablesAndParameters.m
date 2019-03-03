% Integer Problem 
% 
% max z = 4*x_1 - x_2
%
%         7*x_1 - 2*x_2 <= 14
%                   x_2 <= 3
%         2*x_1 - 2*x_2 <= 3
%
%                     x in Z^2_+ 
%
% pure optimization template for linear problem:
% 
% max z = c*x
%
% s.t. Ax <= b
%       x >= 0


incumbent.solution = [ -Inf, -Inf];
incumbent.upperBound = Inf;
incumbent.lowerBound = -Inf;

% formulation from branch-and-bound example in integer programming, wolsey.
originalFormulation.c = [ 4 -1 ];
originalFormulation.A = [ 7 -2 ; 0 1 ; 2 -2 ];
originalFormulation.b = [ 14 ; 3 ; 3 ];
originalFormulation.Aeq = [];
originalFormulation.beq = [];
originalFormulation.lowerBound = [0 0];
originalFormulation.upperBound = [];
originalFormulation.minOrMax = 'max';
originalFormulation.solutionDimension = 2;

% exercise 2 - branch-and-bound chapter, wolsey
% originalFormulation.c = [ 9 5 ];
% originalFormulation.A = [ 4 9 ; 1 0 ; -1 3 ; 3 2 ];
% originalFormulation.b = [ 35 ; 6 ; -1 ; 19];
% originalFormulation.Aeq = [];
% originalFormulation.beq = [];
% originalFormulation.lowerBound = [0 0];
% originalFormulation.upperBound = [];
% originalFormulation.minOrMax = 'max';
% originalFormulation.solutionDimension = 2;

currentFormulation = [];

currentLeaf.treeLevel = 0;
currentLeaf.additionalRestrictions.A = [];
currentLeaf.additionalRestrictions.b = [];
currentLeaf.additionalRestrictions.Aeq = [];
currentLeaf.additionalRestrictions.beq = [];
currentLeaf.upperBound = Inf;
currentLeaf.lowerBound = -Inf;
currentLeaf.solution = [];
currentLeaf.linProgFlag = [];
currentLeaf.prunedBy = [];

listOfActiveLeaves = currentLeaf;
