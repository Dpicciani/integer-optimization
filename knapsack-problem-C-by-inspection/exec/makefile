# 30/09/2017 16:40, Tijuca, Rio de Janeiro
# Author: Douglas Picciani de Souza
#
# This simple makefile was written to be used 
# to compile the files for the metaheuristic
# class work.
#
# I consider three folders as the directory
# structure: 
# 	- include: all new .h files
# 	- obj: all .o files 
# 	- src: all .c files

CC = gcc

_DEPS = mochila.h numericalRecipes.h
_OBJS_EXEC_ = mochila.o mochilaFunctions.o numericalRecipes.o
_EXEC = mochila_frac

IDIR = ../include
SDIR = ../src
ODIR = ../obj
DDIR = ../database

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

IFLAGS = -I$(IDIR)
WFLAG = -Wall

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS_EXEC_))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(WFLAG) -c -o $@ $< $(IFLAGS) 

mochila_frac: $(OBJS)
	$(CC) $(WFLAG) -o mochila_frac $(OBJS) $(IFLAGS) 


.PHONY: clean

clean:
	clear
	rm -f $(ODIR)/*.o $(ODIR)/*~  $(_EXEC) $(SDIR)/*~ $(IDIR)/*~ *~ $(DDIR)/*~
