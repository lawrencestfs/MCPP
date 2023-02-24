####################################################
###################### MAKE ########################
####################################################

PATHPROBLEM = ./src/problem
PATHEXACT = ./src/exato
PATHEXEC = ./bin
PATHSRC = ./src

EXECUTAVEL = MCPP.run
PATHTEMP = ./.temp

all:
	mkdir -p $(PATHEXEC)
	mkdir -p $(PATHTEMP)
	make $(EXECUTAVEL)

# Juntando todos os objetos e gerando o programa
$(EXECUTAVEL): $(PATHTEMP)/Exato.o $(PATHTEMP)/Solution.o $(PATHTEMP)/BitArray.o $(PATHTEMP)/NodeList.o $(PATHTEMP)/Node.o $(PATHTEMP)/EdgeList.o $(PATHTEMP)/Edge.o $(PATHTEMP)/Instance.o $(PATHTEMP)/main.o
	$(CPP) $(CCFLAGS) $(PATHTEMP)/Exato.o $(PATHTEMP)/Solution.o $(PATHTEMP)/BitArray.o $(PATHTEMP)/NodeList.o $(PATHTEMP)/Node.o $(PATHTEMP)/EdgeList.o $(PATHTEMP)/Edge.o $(PATHTEMP)/Instance.o $(PATHTEMP)/main.o $(CCLNFLAGS) $(LSFLAGS) -o $(PATHEXEC)/$(EXECUTAVEL) -lrt


# Compiling main file:

$(PATHTEMP)/main.o: $(PATHSRC)/main.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/main.cpp -o $(PATHTEMP)/main.o


# Compiling problem package:

$(PATHTEMP)/Instance.o: $(PATHPROBLEM)/Instance.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/Instance.cpp -o $(PATHTEMP)/Instance.o

$(PATHTEMP)/Edge.o: $(PATHPROBLEM)/Edge.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/Edge.cpp -o $(PATHTEMP)/Edge.o

$(PATHTEMP)/EdgeList.o: $(PATHPROBLEM)/EdgeList.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/EdgeList.cpp -o $(PATHTEMP)/EdgeList.o

$(PATHTEMP)/Node.o: $(PATHPROBLEM)/Node.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/Node.cpp -o $(PATHTEMP)/Node.o

$(PATHTEMP)/NodeList.o: $(PATHPROBLEM)/NodeList.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/NodeList.cpp -o $(PATHTEMP)/NodeList.o

$(PATHTEMP)/BitArray.o: $(PATHPROBLEM)/BitArray.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/BitArray.cpp -o $(PATHTEMP)/BitArray.o

$(PATHTEMP)/Solution.o: $(PATHPROBLEM)/Solution.cpp
	$(CPP) $(CCFLAGS) -c $(PATHPROBLEM)/Solution.cpp -o $(PATHTEMP)/Solution.o


# Compiling exact package:

$(PATHTEMP)/Exato.o: $(PATHEXACT)/Exato.cpp
	$(CPP) $(CCFLAGS) -c $(PATHEXACT)/Exato.cpp -o $(PATHTEMP)/Exato.o


####################################################
###################### CLEAN #######################
####################################################

clean:
	rm -rf $(PATHEXEC)
	rm -rf $(PATHTEMP)

####################################################
####################### CPLEX ######################
####################################################

##### CPLEX CONFIGURATION's
# System architecture
SYSTEM = x86-64_linux
# Static library format for Cplex
LIBFORMAT = static_pic
# Cplex directory
CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1210/cplex
# Concert directory
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1210/concert

##### CPLEX DIRECTIVE's
# Cplex static libraries directory
CPLEXLIBDIR = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Concert static libraries directory
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Cplex header's directory
CPLEXINCDIR = $(CPLEXDIR)/include
# Concert header's directory
CONCERTINCDIR = $(CONCERTDIR)/include

####################################################
##################### COMPILER #####################
####################################################

##### COMPILER CONFIGURATION's
# Compiler
CPP = g++ -std=c++11
# Compilation parameters
CCOPT = -m64 -O3 -g -fPIC -fexceptions -DIL_STD #-DDEBUG
## Include libraries identifiers
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -m64 -lm -pthread -ldl
# Header's include path
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
