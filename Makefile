CC = gcc
LD = gcc

CFLAGS  = -Wall -g -std=c99 
LDFLAGS = -g
LDLIBS  = -lz

SRCS = main.c dag.c nf.c
OBJS = main.o dag.o nf.o

TARGETS = solver.out 

all: ${TARGETS}

solver.out: $(OBJS) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< 

%.d: %.c
	gcc -MM -MF $@ $<

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f all.png *.d *.o $(TARGETS) 