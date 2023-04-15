CC := gcc
FLAGS := -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -Wno-unused-function
INC := -I includes/

EXEC_SERVER := bin/monitor
SRC_SERVER := src/monitor.c src/common.c
OBJ_SERVER := $(SRC_SERVER:src/%.c=obj/monitor/%.o)

EXEC_CLIENT := bin/tracer
SRC_CLIENT := src/tracer.c src/parser.c src/common.c src/requests.c
OBJ_CLIENT := $(SRC_CLIENT:src/%.c=obj/tracer/%.o)

BUILD_DIR := $(shell mkdir -p bin obj/monitor obj/tracer)

.PHONY: all
all: $(EXEC_SERVER) $(EXEC_CLIENT)

$(EXEC_SERVER): $(OBJ_SERVER)
	@$(CC) $(FLAGS) $^ -o $@ ; echo "[Compiling] $@"

obj/monitor/%.o: src/%.c
	@$(CC) $(FLAGS) -c $< $(INC) -o $@ ; echo "[Linking] $@"

$(EXEC_CLIENT): $(OBJ_CLIENT)
	@$(CC) $(FLAGS_CLIENT) $^ -o $@ ; echo "[Compiling] $@"

obj/tracer/%.o: src/%.c
	@$(CC) $(FLAGS) -c $< $(INC) -o $@ ; echo "[Compiling] $@"


.PHONY: clean
clean:
	@unlink request_pipe ; echo "[Cleaning] Making sure the pipe is closed"
	@rm -rf bin/ ; echo "[Cleaning] bin/"
	@rm -rf obj/ ; echo "[Cleaning] obj/"