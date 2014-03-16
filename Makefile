CC = gcc
CFLAGS = -Wall -g -pthread

PROGRAMS = sudoku-v \
           sudoku-d \
           sudoku-c

all: $(PROGRAMS)

clean:
	rm -f *.o *~ $(PROGRAMS)
