function [maxFlow, model] = fordFulkerson(model)

path = [];
model.flow(7) = model.Gf.c.f(7);
path = searchForAPath(model, 0, path);

maxFlow = 0;
end