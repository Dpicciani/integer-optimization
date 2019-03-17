# 30/09/2017 16:40, Tijuca, Rio de Janeiro
# Author: Douglas Picciani de Souza
#
# This simple makefile was written to be used 
# to compile the files for the metaheuristic
# class work.
#
# I consider three folders as the directory
# structure: 
# 	- include: all .h files
# 	-     obj: all .o files 
# 	-     src: all .c files

PLATFORM = linux64

CC  = gcc
CPP = g++

CARGS = -m64 -g

_DEPS = uflpBranchAndCut.h numericalRecipes.h errorHandling.h separationAlgorithms.h
_OBJS = uflpBranchAndCut.o uflpBranchAndCutFunctions.o numericalRecipes.o separationAlgorithms.o
_BIN = uflpBranchAndCut

GIDIR = $(GUROBI_HOME)/include/
GLIB = -L$(GUROBI_HOME)/lib/ -lgurobi81 

BFDIR = buffer
BDIR  = bin
IDIR  = include
SDIR  = src
ODIR  = obj
DDIR  = database
OUDIR = output

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

IFLAGS = -I$(IDIR)
WFLAG  = -Wall
GFLAGS = -I$(GIDIR) $(GLIB) -lm 

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(WFLAG) -c -o $@ $< $(IFLAGS) $(GFLAGS)

uflpBranchAndCut: $(OBJS)
	$(CC) $(WFLAG) -o $(BDIR)/uflpBranchAndCut $(OBJS) $(IFLAGS) $(GFLAGS)


.PHONY: clean

clean:
	clear
	rm -f $(ODIR)/*.o $(ODIR)/*~  $(_BIN) $(SDIR)/*~ $(IDIR)/*~ *~ $(DDIR)/*~ $(DDIR)/$(_BIN).out $(DDIR)/$(_BIN).lp  $(DDIR)/$(_BIN).log $(BFDIR)/*.lp $(BDIR)/$(_BIN) $(DDIR)/$(OUDIR)/*
