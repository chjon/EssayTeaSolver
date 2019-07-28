CC = g++
LD = g++

CFLAGS  = -Wall -g -std=c++14 
LDFLAGS = -g

SRCS = \
	tseitin_transform_main.cpp dpll_main.cpp vertex_cover_main.cpp \
	CNF_Clause.cpp CNF_Formula.cpp NNF_Formula.cpp \
	DPLL.cpp Vertex_Cover.cpp
OBJS = CNF_Clause.o CNF_Formula.o NNF_Formula.o Graph.o DPLL.o Vertex_Cover.o

TARGETS = TT DPLL VC

all: TT DPLL VC

TT: tseitin_transform_main.o $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS) 

DPLL: dpll_main.o $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS) 

VC: vertex_cover_main.o $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS) 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< 

%.d: %.cpp
	gcc -MM -MF $@ $<

-include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGETS) 