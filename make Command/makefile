CC = gcc
WARNING_FLAGS = -g -Wall -Wextra -pedantic
EXE = 537make

all: main.o parser.o process.o graph.o
	$(CC) $(OPTS) -o $(EXE) main.o parser.o process.o graph.o


main.o: main.c parser.c parser.h process.c process.h graph.c graph.h
	$(CC) $(WARNING_FLAGS) -c main.c

parser.o: parser.c parser.h 
	$(CC) $(WARNING_FLAGS) -c parser.c

process.o: process.c process.h parser.c parser.h graph.c graph.h
	$(CC) $(WARNING_FLAGS) -c process.c
	
graph.o: graph.c graph.h process.c process.h parser.c parser.h
	$(CC) $(WARNING_FLAGS) -c graph.c

clean:
	rm -f $(EXE) *.o