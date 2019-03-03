x = currentSolution.x;
y = currentSolution.y;

for j1 = 1:currentFormulation.n
for j2 = (j1+1):currentFormulation.n
for j3 = (j2+1):currentFormulation.n

initialize3ClientStructure;

for i = 1:currentFormulation.m

  r(1)  = 2*(x(i,j1) + x(i,j2) - y(i));
  r(2)  = 2*(x(i,j1) + x(i,j3) - y(i));
  r(3)  = 2*(x(i,j2) + x(i,j3) - y(i));
  r(4) = x(i,j1) + x(i,j2) + x(i,j3) - y(i);

  if max(r) > 0
    
    maxIndices = find( r == max(r));
    [~, minSetIndex] = min(threeClient.SLength(maxIndices));
    idx = maxIndices(minSetIndex);
    switch idx
      case 1,
        threeClient.S12 = union(threeClient.S12, i);
        threeClient.SLength(1) = threeClient.SLength(1) + 1;
      case 2,
        threeClient.S13 = union(threeClient.S13, i);
        threeClient.SLength(2) = threeClient.SLength(2) + 1;
      case 3,
        threeClient.S23 = union(threeClient.S23, i);
        threeClient.SLength(3) = threeClient.SLength(3) + 1;
      case 4,
        threeClient.S123 = union(threeClient.S123, i);
        threeClient.SLength(4) = threeClient.SLength(4) + 1;
      otherwise
        display('Error in separation algorithm');
    end
    
    threeClient.violationDegree = threeClient.violationDegree + max(r>0);
    threeClient.token1 = sum([threeClient.S12 threeClient.S13 threeClient.S23 threeClient.S123]);
    threeClient.token2 = sum(threeClient.J);
    threeClient.token3 = sum(threeClient.SLength);
  end
  
end

isThisRestrictionAcceptable;
if acceptance == true
  currentlyViolatedActive.Set{inequalityIndex} = threeClient;
  currentlyViolatedActive.dS(inequalityIndex) = 0; 
  inequalityIndex = inequalityIndex + 1;
end

% if isempty(find(threeClient.SLength(1:3) == 0, 1)) &&...
%    threeClient.violationDegree >= 4
%   currentlyViolatedActive.Set{inequalityIndex} = threeClient;
%   currentlyViolatedActive.dS(inequalityIndex) = 0; 
%   inequalityIndex = inequalityIndex + 1;
% end

end
end
end

clear x y i j1 j2 j3;