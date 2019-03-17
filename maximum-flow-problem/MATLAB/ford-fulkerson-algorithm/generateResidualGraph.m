function model = generateResidualGraph(model)

Gf.V = model.G.V;
Gf.E.f = []; % forward edges
Gf.c.f = [];
Gf.E.b = []; % backward edges
Gf.c.b = [];


for i = 1:size(model.G.E,1)
  if model.flow(i) < model.G.c(i)
    Gf.E.f = [Gf.E.f ; model.G.E(i,:)];
    Gf.c.f = [Gf.c.f; model.G.c(i) - model.flow(i)];
  end
end

for i = 1:size(model.G.E,1)
  if model.flow(i) > 0
    Gf.E.b = [Gf.E.b ; model.G.E(i,end:-1:1)];
    Gf.c.b = [Gf.c.b; model.flow(i)];
  end
end

Gf.explored = zeros(size(Gf.V,1), 1);

model.Gf = Gf;

end