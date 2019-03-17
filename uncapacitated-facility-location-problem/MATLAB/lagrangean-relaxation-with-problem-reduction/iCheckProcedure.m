function [ pExists, I_ ] = iCheckProcedure( facility, client, I,...
                                            supplyingCosts, facilityCosts )

pExists = false;
clientsQuantity = size(supplyingCosts, 2);

% first definition
observedSupplyingCosts = supplyingCosts(I{client}, client);
I_ = I{client}(observedSupplyingCosts >= supplyingCosts(facility,client));

if isempty(I_)
  pExists = 1;
  return;
end

% second definition
for clientQ = 1:clientsQuantity
  clientClosedFacilities = setdiff(I{client}, I_);
  facilityP = setdiff(I{clientQ},clientClosedFacilities);
  if isempty(facilityP)
    minorSupplyingQCosts{clientQ} = 1e10;
  else
    minorSupplyingQCosts{clientQ} = min(supplyingCosts(facilityP,clientQ));
  end
end

% comparison to eliminate
facilitySearchSet = setdiff( I{client} ,I_ );
for facilityP = facilitySearchSet
  sumTerm = 0;
  for clientQ = 1:clientsQuantity
    if ismember(facilityP, I{clientQ})
      savedCost = minorSupplyingQCosts{clientQ} - supplyingCosts(facilityP,clientQ);
      sumTerm = sumTerm + max(0, savedCost);
    end
  end
  
  if facilityCosts(facilityP) < sumTerm
    pExists = true;
    return;
  end
end
end

