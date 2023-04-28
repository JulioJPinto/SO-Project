CC := gcc

FLAGS := -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -Wno-unused-result
LDFLAGS =

ifeq ($(DEBUG), 1)
	FLAGS += -O0 -g
else
	FLAGS += -O3
endif

COMMON_DIR = common
MONITOR_DIR = monitor
TRACER_DIR = tracer

COMMON_SRCS := $(wildcard $(COMMON_DIR)/*.c)
MONITOR_SRCS := $(wildcard $(MONITOR_DIR)/*.c)
TRACER_SRCS := $(wildcard $(TRACER_DIR)/*.c)

OBJ_DIR = obj
COMMON_OBJ_DIR = $(OBJ_DIR)/common
MONITOR_OBJ_DIR = $(OBJ_DIR)/monitor
TRACER_OBJ_DIR = $(OBJ_DIR)/tracer

COMMON_OBJS := $(patsubst $(COMMON_DIR)/%.c,$(COMMON_OBJ_DIR)/%.o,$(COMMON_SRCS))
MONITOR_OBJS := $(patsubst $(MONITOR_DIR)/%.c,$(MONITOR_OBJ_DIR)/%.o,$(MONITOR_SRCS))
TRACER_OBJS := $(patsubst $(TRACER_DIR)/%.c,$(TRACER_OBJ_DIR)/%.o,$(TRACER_SRCS))

COMMON_INCLUDES := -I$(COMMON_DIR)
MONITOR_INCLUDES := $(COMMON_INCLUDES) -I$(MONITOR_DIR)
TRACER_INCLUDES := $(COMMON_INCLUDES) -I$(TRACER_DIR)

BIN_DIR = bin

all: monitor tracer

monitor: $(MONITOR_OBJS) $(COMMON_OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o bin/$@

tracer: $(TRACER_OBJS) $(COMMON_OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o bin/$@

$(COMMON_OBJ_DIR)/%.o: $(COMMON_DIR)/%.c | $(COMMON_OBJ_DIR)
	$(CC) $(CFLAGS) $(COMMON_INCLUDES) -c $< -o $@

$(MONITOR_OBJ_DIR)/%.o: $(MONITOR_DIR)/%.c | $(MONITOR_OBJ_DIR)
	$(CC) $(CFLAGS) $(MONITOR_INCLUDES) -c $< -o $@

$(TRACER_OBJ_DIR)/%.o: $(TRACER_DIR)/%.c | $(TRACER_OBJ_DIR)
	$(CC) $(CFLAGS) $(TRACER_INCLUDES) -c $< -o $@

$(COMMON_OBJ_DIR):
	mkdir -p $(COMMON_OBJ_DIR)

$(MONITOR_OBJ_DIR):
	mkdir -p $(MONITOR_OBJ_DIR)

$(TRACER_OBJ_DIR):
	mkdir -p $(TRACER_OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: clean
clean: cleanpipe
	@rm -rf bin/ ; echo "[Cleaning] bin/"
	@rm -rf obj/ ; echo "[Cleaning] obj/"

cleanpipe:
	@find . -type p -delete ; echo "[Cleaning] Making sure the pipes are closed"

format:
	clang-format --style="{IndentWidth: 4, TabWidth: 4}" -i src/*.c includes/*.h
