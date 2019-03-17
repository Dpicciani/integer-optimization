
delta = currentFormulation.c + repmat( currentFormulation.F,...
                                       [1 currentFormulation.n] );

delta = min(delta);