function vec = binaryVector(dimension, nonZeroIndex)
  vec = zeros(1, dimension);
  vec(nonZeroIndex) = 1;
end