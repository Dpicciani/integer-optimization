function x = numericalErrorsCorrection(x)

roundedX = round(x);

errorVector = abs(roundedX - x);

selectedPositionsToRound = find(errorVector < 10e-4);

x(selectedPositionsToRound) = roundedX(selectedPositionsToRound);

end