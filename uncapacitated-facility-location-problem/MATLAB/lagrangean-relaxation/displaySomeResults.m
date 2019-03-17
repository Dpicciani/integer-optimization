
% display best bounds found, optimal gap size, step length f
fprintf('upper bound: %14.5f | lower bound: %14.5f ',...
  incumbent.minimumUpperBoundFound,incumbent.maximumLowerBoundFound);
% fprintf('| gap size: %14.5f ', ...
%   incumbent.minimumUpperBoundFound - incumbent.maximumLowerBoundFound);
fprintf('| opt gap(%%): %8.5f ', ...
  (incumbent.minimumUpperBoundFound - incumbent.maximumLowerBoundFound)*100/incumbent.maximumLowerBoundFound);
fprintf('| open Warehouses: %3d ', ...
  sum(currentSolution.y));
fprintf('| iter = %4d ', LLBP.totalSubgradientIterations);
fprintf('| f = %.6f ', LLBP.f);
fprintf('| norm(subGrad) = %14.5f ', norm(LLBP.subGrad));
fprintf('| LLBP solved = %3s ', currentSolution.LLBPSolved);
fprintf('| currentLagSol: %12.5f\n', ...
  currentSolution.lowerBound);

% display subgradient vector
% fprintf('subGrad: ');
% for subGradCoordinate = 1:currentFormulation.m
%   fprintf('%2d ',LLBP.subGrad(subGradCoordinate));
% end
% fprintf('\n');

% display current lagrangean solution
% fprintf('lagSol: ');
% for subGradCoordinate = 1:currentFormulation.m
%   fprintf('%2d ',currentSolution.y(subGradCoordinate));
% end
% fprintf('\n');