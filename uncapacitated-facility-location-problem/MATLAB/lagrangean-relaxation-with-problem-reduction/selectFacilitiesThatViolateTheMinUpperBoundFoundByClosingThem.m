
selectedFacilityIndices =...
  find( problemReduction.maxLBByClosedFacility >= ...
        incumbent.minimumUpperBoundFound + 1e-4 );