function yAtomicTerm = y3ClientTerm( i, currentSolution, restriction)

yAtomicTerm = 0;

iBelongsToS12  = ismember( i, restriction.S12);
iBelongsToS13  = ismember( i, restriction.S13);
iBelongsToS23  = ismember( i, restriction.S23);
iBelongsToS123 = ismember( i, restriction.S123);

if ~iBelongsToS12 &&...
    ~iBelongsToS13 &&...
    ~iBelongsToS23 &&...
    ~iBelongsToS123
  return;
end

if iBelongsToS12 || iBelongsToS13 || iBelongsToS23
  yAtomicTerm = -2*currentSolution.y(i);
else
  yAtomicTerm = -currentSolution.y(i);
end
end