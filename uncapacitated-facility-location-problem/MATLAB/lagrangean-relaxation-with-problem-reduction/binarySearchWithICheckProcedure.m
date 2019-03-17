function I_ = binarySearchWithICheckProcedure( facilities, client, I,...
                                               supplyingCosts,...
                                               facilityCosts )

getTheMiddleFacilityFromAFacilityList;

iCheckProcedureApplication;

if facilitiesQuantity == 1
  if ~pExists, I_ = []; end
  return;
end
                                 
if pExists
  facilitiesShrinked = facilities(1:facilityPosition);
else
  facilitiesShrinked = facilities((facilityPosition+1):end);
end
       
I_ = binarySearchWithICheckProcedure( facilitiesShrinked, client, I,...
                                      supplyingCosts, facilityCosts );
end