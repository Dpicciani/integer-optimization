
originalFormulation.m = []; % number of facilities
originalFormulation.n = []; % number of clients
originalFormulation.F = []; % facilities costs
originalFormulation.c = []; % supplying costs
originalFormulation.databasePath = ['D:\local-database\integer_',...
                                    'problems\uncapacited-warehouse',...
                                    '-location\cap72.txt'];

% load(['D:\local-database\integer_problems\uncapacited-warehouse',...
%     '-location\capc_mod.mat']);
getOriginalFormulationFromDatabaseFile;

LLBP.s  = [];       % lagrangean multipliers
LLBP.lc = [];       % lagrangean costs
LLBP.rlc = [];      % reduced lagrangean costs
LLBP.subGrad = []; 	% subgradients
LLBP.f = 2;         % step length for the subgradient optimization
LLBP.T = [];        % step size for the lagrangean multipliers
LLBP.tau = 1.05;    % upper bound multiplier for step size T
LLBP.subgradientIterations = 0;
LLBP.totalSubgradientIterations = 0;
LLBP.maximumSubgradientIterations = 1e3;
LLBP.maximumSubgradientIterationsWithoutLBImprovements =...
  LLBP.maximumSubgradientIterations/20;
LLBP.minimumStepLength = 1e-8;

initializeLagrangeanMultipliers;

incumbent.feasibleSolution.x = [];
incumbent.feasibleSolution.y = [];
incumbent.minimumUpperBoundFound = Inf;
incumbent.maximumLowerBoundFound = -Inf;

currentSolution.x = [];
currentSolution.y = [];
currentSolution.lowerBound = -Inf;
currentSolution.feasible.x = [];
currentSolution.feasible.y = [];
currentSolution.upperBound = Inf;

problemReduction.openFacilities = [];
problemReduction.closedFacilities = [];
problemReduction.maxLBByOpenFacility = -Inf(1, originalFormulation.m);
problemReduction.maxLBByClosedFacility = -Inf(1, originalFormulation.m);
problemReduction.fixedDemands = -ones(  originalFormulation.m,...
                                              originalFormulation.n  );
problemReduction.degree = [];

currentFormulation = originalFormulation;