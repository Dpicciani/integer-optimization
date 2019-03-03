LLBP.subGrad = ones(1, currentFormulation.n) - sum(currentSolution.x);

numberOfViolatedRestrictions = length(previouslyViolatedActiveSet);

for violatedRestrictionIndex = 1:numberOfViolatedRestrictions
  restriction = previouslyViolatedActiveSet{violatedRestrictionIndex};
  LLBP.dSubGrad(violatedRestrictionIndex) =...
    LLBP.dSubGradFunction(restriction, currentSolution);
end


numberOfViolatedRestrictions = length(currentlyViolatedActive.Set);

for violatedRestrictionIndex = 1:numberOfViolatedRestrictions
  restriction = currentlyViolatedActive.Set{violatedRestrictionIndex};
  currentlyViolatedActive.dSubGrad(violatedRestrictionIndex) =...
    LLBP.dSubGradFunction(restriction, currentSolution);
end
