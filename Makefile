NC =	 \033[0m
YELLOW = \033[1;33m
GREEN =  \033[1;32m

CC = gcc
LD = gcc

CFLAGS = -std=c17 -Wall -Werror -pedantic -O3 -ggdb

BIN = bin

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

EXE = bfi

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	@ mkdir -p $(BIN)
	@ echo -e "$(GREEN)LINKING$(NC) $(EXE)"
	@ $(CC) $^ -o $(BIN)/$@ $(LDFLAGS)

%.o: %.c
	@ echo -e "$(GREEN)COMPILING$(NC) $<"
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ echo -e "$(YELLOW)CLEANING PROJECT"
	@ rm -rf $(BIN) $(OBJ)
