LLBP.s = originalFormulation.c;
LLBP.s( LLBP.s <= 0 ) = Inf;    % exclude non-positive values
LLBP.s = min(LLBP.s);