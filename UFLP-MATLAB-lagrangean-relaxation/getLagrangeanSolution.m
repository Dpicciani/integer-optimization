currentSolution.y = zeros(currentFormulation.m, 1);
currentSolution.y(IndexesOfChosenRLC,1) = 1;

clear IndexesOfChosenRLC;