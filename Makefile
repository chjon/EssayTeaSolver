CC = g++
LD = g++

CFLAGS  = -Wall -g -std=c++14 
LDFLAGS = -g
LDLIBS  = -lz

SRCS = main.cpp CNF_Clause.cpp CNF_Formula.cpp
OBJS = main.o CNF_Clause.o CNF_Formula.o

TARGETS = solver.out 

solver.out: $(OBJS) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< 

%.d: %.cpp
	gcc -MM -MF $@ $<

-include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f all.png *.d *.o $(TARGETS) 