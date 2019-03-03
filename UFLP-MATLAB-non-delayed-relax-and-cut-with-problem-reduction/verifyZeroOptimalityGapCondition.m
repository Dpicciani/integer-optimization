if norm( incumbent.minimumUpperBoundFound -...
         incumbent.maximumLowerBoundFound ) < 1e-6
  decision = true;
end