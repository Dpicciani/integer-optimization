clc; clear all; close all;

% set maximum flow problem parameters
model.G.V = [0;1;2;3;4;5];
model.G.E = [0,1 ; 0,3 ; 1,2 ; 1,3 ; 1,4 ; 4,2 ; 2,5 ; 4,5];
model.G.c = [10;10;4;2;8;6;10;10];

model.flow = zeros(size(model.G.E,1),1);

model = generateResidualGraph(model);

% solve with ford-fulkerson algorithm
[maxFlow, model] = fordFulkerson(model);

% show results