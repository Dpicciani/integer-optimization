acceptance = false;

if isempty(find(threeClient.SLength(1:3) == 0, 1))
  
  maxVariables = 2;
  
  v1 = threeClient.J(1);
  v2 = threeClient.J(2);
  v3 = threeClient.J(3);
  for s = threeClient.S12
    if (variablesApproachedByViolation.x(s,v1) <= maxVariables ||...
        variablesApproachedByViolation.x(s,v2) <= maxVariables) &&...
        acceptance == false
      acceptance = true;
      break;
    end
  end
  
  if acceptance == false
  for s = threeClient.S13
    if (variablesApproachedByViolation.x(s,v1) <= maxVariables ||...
        variablesApproachedByViolation.x(s,v3) <= maxVariables) &&...
        acceptance == false
      acceptance = true;
      break;      
    end
  end
  end
  
  if acceptance == false
  for s = threeClient.S23
    if (variablesApproachedByViolation.x(s,v2) <= maxVariables ||...
        variablesApproachedByViolation.x(s,v3) <= maxVariables) &&...
        acceptance == false
      acceptance = true;
      break;
    end
  end
  end
  
  if acceptance == true
    for s = threeClient.S12
      variablesApproachedByViolation.x(s,v1) = variablesApproachedByViolation.x(s,v1) + 1;
      variablesApproachedByViolation.x(s,v2) = variablesApproachedByViolation.x(s,v2) + 1;
    end
    
    for s = threeClient.S13
      variablesApproachedByViolation.x(s,v1) = variablesApproachedByViolation.x(s,v1) + 1;
      variablesApproachedByViolation.x(s,v3) = variablesApproachedByViolation.x(s,v3) + 1;
    end
    
    for s = threeClient.S23
      variablesApproachedByViolation.x(s,v2) = variablesApproachedByViolation.x(s,v2) + 1;
      variablesApproachedByViolation.x(s,v3) = variablesApproachedByViolation.x(s,v3) + 1;
    end
  end
  
  clear v1 v2 v3 s maxVariables;
end

