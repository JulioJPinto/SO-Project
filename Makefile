CC := gcc
FLAGS := -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -Wno-unused-result
INC := -I includes/

ifeq ($(DEBUG), 1)
	FLAGS += -O0 -g
else
	FLAGS += -O3
endif

EXEC_SERVER := bin/monitor
SRC_SERVER := src/monitor.c src/common.c src/requests.c src/interface.c src/errors.c
OBJ_SERVER := $(SRC_SERVER:src/%.c=obj/monitor/%.o)

EXEC_CLIENT := bin/tracer
SRC_CLIENT := src/tracer.c src/common.c src/requests.c src/input.c src/errors.c src/parser.c src/execute.c
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
	@find . -type p -delete ; echo "[Cleaning] Making sure the pipes are closed"
	@rm -rf bin/ ; echo "[Cleaning] bin/"
	@rm -rf obj/ ; echo "[Cleaning] obj/"

cleanpipe:
	@find . -type p -delete ; echo "[Cleaning] Making sure the pipes are closed"

format:
	clang-format -i src/*.c includes/*.h