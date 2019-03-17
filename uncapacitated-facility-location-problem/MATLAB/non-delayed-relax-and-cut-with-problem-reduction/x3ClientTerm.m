function xAtomicTerm = x3ClientTerm( i, j, currentSolution, restriction)

xAtomicTerm = 0;

iBelongsToS12  = ismember( i, restriction.S12);
iBelongsToS13  = ismember( i, restriction.S13);
iBelongsToS23  = ismember( i, restriction.S23);
iBelongsToS123 = ismember( i, restriction.S123);
jBelongsToJ    = ismember( j, restriction.J);

if ( ~iBelongsToS12 &&...
     ~iBelongsToS13 &&...
     ~iBelongsToS23 &&...
     ~iBelongsToS123 ) ||...
     ~jBelongsToJ
  
  return;
end

if iBelongsToS12 || iBelongsToS13 || iBelongsToS23
  xAtomicTerm = 2*currentSolution.x(i,j);
else
  xAtomicTerm = currentSolution.x(i,j);
end
end