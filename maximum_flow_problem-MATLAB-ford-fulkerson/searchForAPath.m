function [path, found, model] = searchForAPath(model, u, path)

% mark u as "explored"
model.Gf.explored(u+1) = 1;

found = 0;

path = [path; u];

incidentEdges.f = find(model.Gf.E.f(:,1) == u);

for edge = 1:size(incidentEdges.f)
  if found == 1
    break;
  end
  i = incidentEdges.f(edge);
  v = model.Gf.E.f(i,2);
  if model.Gf.explored(v+1) == 0 &&...
     model.Gf.c.f(i) ~= model.flow(i)
    [path, found, model] = searchForAPath(model, v, path);
  end
  
end

if path(end) == length(model.G.V)-1
  found = 1;
end

end