
originalFormulation.m = []; % number of facilities
originalFormulation.n = []; % number of clients
originalFormulation.F = []; % facilities costs
originalFormulation.c = []; % supplying costs
originalFormulation.databasePath = ['D:\local-database\integer_',...
                                    'problems\uncapacited-warehouse',...
                                    '-location\cap73.txt'];

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
LLBP.maximumSubgradientIterations = 1000;
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

LLBP.dS = [];                        % dynamic lagrangean multipliers
LLBP.dX = @x3ClientTerm;             % dynamic x factor
LLBP.dY = @y3ClientTerm;             % dynamic y factor
LLBP.dI = @independent3ClientTerm;   % dynamic independent factor
LLBP.dSubGrad = [];    	             % dynamic subgradients
LLBP.dSubGradFunction = @threeClientSubGradient; 

currentlyViolatedActive.Set = {};
currentlyViolatedActive.dS = [];
currentlyViolatedActive.dSubGrad = [];

previouslyViolatedActiveSet = {};
inequalityIndex = 1;

variablesApproachedByViolation.x = zeros(originalFormulation.m,...
                                         originalFormulation.n);
variablesApproachedByViolation.y = zeros(originalFormulation.m);

% 3-client inequality template
threeClient.S12 = [];
threeClient.S13 = [];
threeClient.S23 = [];
threeClient.S123 = [];
threeClient.J = [];
threeClient.violationDegree = 0;
threeClient.SLength = [0 0 0 0];