#!/bin/bash

make clean
make
./bin/uflpBranchAndBound
vim buffer/uflpModel.lp

