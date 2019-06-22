CC = g++
LD = g++

CFLAGS  = -Wall -g -std=c++14 
LDFLAGS = -g
LDLIBS  = -lz

SRCS = NNF_to_CNF.cpp dpll_main.cpp CNF_Clause.cpp CNF_Formula.cpp NNF_Formula.cpp DPLL.cpp
OBJS = CNF_Clause.o CNF_Formula.o NNF_Formula.o DPLL.o

TARGETS = NNF_to_CNF DPLL

all: NNF_to_CNF DPLL	

NNF_to_CNF: NNF_to_CNF.o $(OBJS)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

DPLL: dpll_main.o $(OBJS)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< 

%.d: %.cpp
	gcc -MM -MF $@ $<

-include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGETS) 