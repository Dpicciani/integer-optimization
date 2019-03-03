if norm( incumbent.minimumUpperBoundFound -...
         incumbent.maximumLowerBoundFound ) < 1e-10
  decision = true;
end