# compiler
CC = g++

# compiler flags
CFLAGS = -std=c++11 -Wall -g

# include directories
INCLUDES = -I include

# source directory
SRC = src

# bin directory
BIN = bin

default: $(BIN)/qcp

$(BIN)/qcp: $(BIN)/main.o $(BIN)/board.o $(BIN)/cell.o $(BIN)/solver.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/cell.o:  $(SRC)/cell.cpp include/cell.h
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

$(BIN)/board.o:  $(SRC)/board.cpp include/board.h
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

$(BIN)/solver.o:  $(SRC)/solver.cpp include/solver.h
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

$(BIN)/main.o:  $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)/qcp

clean:
	rm $(BIN)/*