# 09/04/2019 02:30, Praia da Bandeira, Ilha do Governador, Rio de Janeiro, RJ, Brazil
# Author: Douglas Picciani de Souza
#
# This makefile was written to compile the code for
# research in the maximum independent set problem
# based on a formulation by cliques from the 
# clique cover problem.
#
# I consider the following directory structure:
#  include/ - all .h files
#      obj/ - all .o files
#      src/ - all .c files
#     data/ - all database files
# mersenne/ - all files related to SIMD-oriented Fast Mersenne Twister

CC = gcc

CARGS = -Wall -O3 -m64 
MARGS = -msse2 -DHAVE_SSE2 -DSFMT_MEXP=607

IDIR  = -Iinclude
IGDIR = -I$(GUROBI_HOME)/include/
MIDIR = -Imersenne/SFMT-src-1.5.1/



bin/maximumIndependentSet: obj/maximumIndependentSet.o \
                           obj/maximumIndependentSetFunctions.o \
	   	           obj/numericalRecipes.o \
		           obj/SFMT.o
	$(CC) $(CARGS) $(MARGS) -o bin/maximumIndependentSet obj/maximumIndependentSet.o obj/maximumIndependentSetFunctions.o obj/numericalRecipes.o obj/SFMT.o -lm 

obj/maximumIndependentSet.o: src/maximumIndependentSet.c \
    	                     include/maximumIndependentSet.h
	$(CC) $(CARGS) $(MARGS) -c -o obj/maximumIndependentSet.o src/maximumIndependentSet.c $(IDIR) $(IGDIR) $(MIDIR)

obj/maximumIndependentSetFunctions.o: src/maximumIndependentSetFunctions.c \
	                              include/maximumIndependentSet.h
	$(CC) $(CARGS) $(MARGS) -c -o obj/maximumIndependentSetFunctions.o src/maximumIndependentSetFunctions.c $(IDIR) $(IGDIR) $(MIDIR)

obj/numericalRecipes.o: src/numericalRecipes.c \
	                include/numericalRecipes.h
	$(CC) $(CARGS) -c -o obj/numericalRecipes.o src/numericalRecipes.c $(IDIR) -lm

obj/SFMT.o: mersenne/SFMT-src-1.5.1/SFMT.c \
	    mersenne/SFMT-src-1.5.1/SFMT.h
	$(CC) $(CARGS) $(MARGS) -c -o obj/SFMT.o mersenne/SFMT-src-1.5.1/SFMT.c $(MIDIR)

.PHONY: clean

clean:
	clear
	rm -f bin/maximumIndependentSet obj/*.o src/*~ include/*.swp /src/*.swp
