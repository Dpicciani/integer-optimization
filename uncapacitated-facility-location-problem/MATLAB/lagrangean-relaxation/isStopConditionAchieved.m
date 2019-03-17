function decision = isStopConditionAchieved( LLBP, incumbent)

decision = false;

verifySubgradientIterationsCondition;
verifySubgradientVectorNormCondition;
verifyZeroOptimalityGapCondition;
verifyStepLenthCondition;

end