CC := gcc

EXEC := monitor
FLAGS := -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -Wno-unused-function
INC := -I includes/
SRC := src/monitor.c src/common.c
OBJ_DIR := obj
OBJ := $(SRC:src/%.c=build/%.o)

EXEC_CLIENT := tracer
FLAGS_CLIENT := $(FLAGS) 
INC_CLIENT := $(INC)
SRC_CLIENT := src/tracer.c src/parser.c src/common.c
OBJ_CLIENT := $(SRC:src/%.c=build/%.o)

BUILD_DIR := $(shell mkdir -p build)

.PHONY: all
all: $(EXEC) $(EXEC_CLIENT)

$(EXEC): $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@ ; echo "[Compiling] $@"

build/%.o: src/%.c
	@$(CC) $(FLAGS) -c $< $(INC) -o $@ ; echo "[Linking] $@"

$(EXEC_CLIENT): $(OBJ_CLIENT)
	@$(CC) $(FLAGS_CLIENT) $^ -o $@ ; echo "[Compiling] $@"

build/%.o: src/%.c
	@$(CC) $(FLAGS_CLIENT) -c $< $(INC_CLIENT) -o $@ ; echo "[Linking] $@"

.PHONY: clean
clean:
	@unlink pipe ; echo "[Cleaning] Making sure the pipe is closed"
	@rm -f client ; echo "[Cleaning] client"
	@rm -f server ; echo "[Cleaning] server"
	@rm -rf build/ ; echo "[Cleaning] build/"