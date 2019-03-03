LLBP.s = LLBP.s + LLBP.T*LLBP.subGrad;

LLBP.dS = max(0, LLBP.dS + LLBP.T*LLBP.dSubGrad);

currentlyViolatedActive.dS = max(0, currentlyViolatedActive.dS + LLBP.T*...
                             currentlyViolatedActive.dSubGrad);

