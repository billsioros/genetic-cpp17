
CC = g++
CCFLAGS = -Wall -Wextra -std=c++17 -g3

LIBS = 

PATH_SRC = ./src
PATH_INC = ./include
PATH_BIN = ./bin
PATH_TEST = ./test

.PHONY: all
all:
	mkdir -p $(PATH_BIN)
	$(CC) -I $(PATH_INC) $(DEFINED) $(CCFLAGS) $(PATH_TEST)/main.cpp $(LIBS) -o $(PATH_BIN)/main.exe

.PHONY: clean
clean:
	@echo
	@echo "*** Purging binaries ***"
	@echo "***"
	rm -rvf $(PATH_BIN)
	@echo "***"
