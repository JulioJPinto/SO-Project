CC := gcc

EXEC := server
FLAGS := -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -Wno-unused-function
INC := -I includes/
SRC := src/server.c src/common.c
OBJ := $(SRC:src/%.c=obj_server/%.o)

EXEC_CLIENT := client
FLAGS_CLIENT := $(FLAGS) 
INC_CLIENT := $(INC)
SRC_CLIENT := src/client.c src/parser.c src/common.c
OBJ_CLIENT := $(SRC:src/%.c=obj_client/%.o)

BUILD_DIR := $(shell mkdir -p obj_client obj_server)

.PHONY: all
all: $(EXEC) $(EXEC_CLIENT)

$(EXEC): $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@ ; echo "[Compiling] $@"

obj_server/%.o: src/%.c
	@$(CC) $(FLAGS) -c $< $(INC) -o $@ ; echo "[Linking] $@"

$(EXEC_CLIENT): $(OBJ_CLIENT)
	@$(CC) $(FLAGS_CLIENT) $^ -o $@ ; echo "[Compiling] $@"

obj_client/%.o: src/%.c
	@$(CC) $(FLAGS_CLIENT) -c $< $(INC_CLIENT) -o $@ ; echo "[Linking] $@"

.PHONY: clean
clean:
	@unlink pipe ; echo "[Cleaning] Making sure the pipe is closed"
	@rm -f client ; echo "[Cleaning] client"
	@rm -f server ; echo "[Cleaning] server"
	@rm -rf obj_client/ ; echo "[Cleaning] obj_client/"
	@rm -rf obj_server/ ; echo "[Cleaning] obj_server/"