if ~isempty( find( currentSolution.feasible.y ,1) )
 currentSolution.upperBound =...
   currentFormulation.c.*currentSolution.feasible.x;

 currentSolution.upperBound =...
   sum(reshape(currentSolution.upperBound,[],1));

 currentSolution.upperBound =...
   currentSolution.upperBound + dot(currentFormulation.F,...
                                    currentSolution.feasible.y);
end                       
                         