
facilitiesQuantity = length(facilities);

if facilitiesQuantity > 1
  facilityPosition = floor(facilitiesQuantity/2);
else
  facilityPosition = 1;
end

facility = I{client}(facilities(facilityPosition));