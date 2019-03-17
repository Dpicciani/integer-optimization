function [x_downer, x_upper] = floorAndCeil(solution, solutionCoordinate)

x_SC = solution(solutionCoordinate);
x_upper = ceil(x_SC);
x_downer = floor(x_SC);
end