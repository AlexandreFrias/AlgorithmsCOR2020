CFLAGS= -Wall -m64 -g -w
CXX=g++
HOME=/home/prof/Documentos/AlgorithmsComputerAppliedMathematics2019
ILOG=/opt/ibm/ILOG/CPLEX_Studio1271
CPPFLAGS= -DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEXLIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: comp0 comp1

comp0:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o teste teste.cpp  $(CPLEXLIB)

comp1:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o kojic kojic.cpp  $(CPLEXLIB)

