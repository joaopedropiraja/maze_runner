CXX := gcc
CXX_FLAGS := -Wall -Wextra -ggdb

BIN := bin
OBJ := obj
SRC := src
INCLUDE := include
EXECUTABLE := main.out

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ -lm

$(OBJ)/%.o: $(SRC)/%.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	-rm -f $(BIN)/*
	-rm -f $(OBJ)/*.o

$(shell mkdir -p $(BIN) $(OBJ))