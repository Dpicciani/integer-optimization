function decision = isStopConditionAchieved( LLBP, incumbent )

decision = false;

verifySubgradientIterationsCondition;
verifySugradientVectorNormCondition;
verifyZeroOptimalityGapCondition;
verifyStepLenthCondition;

end