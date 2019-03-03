
for i = 1:length(zeroPVAdSIndices)
  
  threeClient = previouslyViolatedActiveSet{i};
  
  v1 = threeClient.J(1);
  v2 = threeClient.J(2);
  v3 = threeClient.J(3);
  
  for s = threeClient.S12
    variablesApproachedByViolation.x(s,v1) = variablesApproachedByViolation.x(s,v1) - 1;
    variablesApproachedByViolation.x(s,v2) = variablesApproachedByViolation.x(s,v2) - 1;
  end
  
  for s = threeClient.S13
    variablesApproachedByViolation.x(s,v1) = variablesApproachedByViolation.x(s,v1) - 1;
    variablesApproachedByViolation.x(s,v3) = variablesApproachedByViolation.x(s,v3) - 1;
  end
  
  for s = threeClient.S23
    variablesApproachedByViolation.x(s,v2) = variablesApproachedByViolation.x(s,v2) - 1;
    variablesApproachedByViolation.x(s,v3) = variablesApproachedByViolation.x(s,v3) - 1;
  end
end