                           
nonZeroCVAdSIndices = find(currentlyViolatedActive.dS ~= 0);
if ~isempty(nonZeroCVAdSIndices)
  
  potentialRestrictions.Set = currentlyViolatedActive.Set(nonZeroCVAdSIndices);
  potentialRestrictions.dS = currentlyViolatedActive.dS(nonZeroCVAdSIndices);
  potentialRestrictions.dSubGrad = currentlyViolatedActive.dSubGrad(nonZeroCVAdSIndices);

  inequalityIndex = inequalityIndex - length(nonZeroCVAdSIndices);
  
  currentlyViolatedActive.Set(nonZeroCVAdSIndices)=[];
  currentlyViolatedActive.dS(nonZeroCVAdSIndices)=[];
  currentlyViolatedActive.dSubGrad(nonZeroCVAdSIndices)=[];
  
  restrictionsToExclude = [];
  for prIndex = 1:length(potentialRestrictions.Set)
   prRestriction = potentialRestrictions.Set{prIndex};
   for pvaIndex = 1:length(previouslyViolatedActiveSet);
    if previouslyViolatedActiveSet{pvaIndex}.token1 == prRestriction.token1
    if previouslyViolatedActiveSet{pvaIndex}.token2 == prRestriction.token2
    if previouslyViolatedActiveSet{pvaIndex}.token3 == prRestriction.token3
      restrictionsToExclude = [restrictionsToExclude prIndex];
    end
    end
    end
   end
  end
  
  potentialRestrictions.Set(restrictionsToExclude) = [];
  potentialRestrictions.dS(restrictionsToExclude) = [];
  potentialRestrictions.dSubGrad(restrictionsToExclude) = [];

  
  previouslyViolatedActiveSet = [previouslyViolatedActiveSet...
    potentialRestrictions.Set];
  
  LLBP.dS = [ LLBP.dS potentialRestrictions.dS];
  
  LLBP.dSubGrad = [ LLBP.dSubGrad potentialRestrictions.dSubGrad];
end

zeroPVAdSIndices = find(abs(LLBP.dS) < 0.1);
if ~isempty(zeroPVAdSIndices)
  
  clearVariablesApproachedByViolation;
  
  previouslyViolatedActiveSet(zeroPVAdSIndices) = [];
  LLBP.dS(zeroPVAdSIndices) = [];
  LLBP.dSubGrad(zeroPVAdSIndices) = [];
end


