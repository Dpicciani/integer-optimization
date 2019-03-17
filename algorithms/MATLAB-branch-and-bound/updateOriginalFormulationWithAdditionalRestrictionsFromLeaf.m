currentFormulation = originalFormulation;

currentFormulation.A = [currentFormulation.A;...
                        currentLeaf.additionalRestrictions.A];
                      
currentFormulation.b = [currentFormulation.b;...
                        currentLeaf.additionalRestrictions.b];
                      
currentFormulation.Aeq = [currentFormulation.Aeq;...
                          currentLeaf.additionalRestrictions.Aeq];
                      
currentFormulation.beq = [currentFormulation.beq;...
                          currentLeaf.additionalRestrictions.beq];
                      

 