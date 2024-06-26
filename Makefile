# Project Settings
debug ?= 1
SRC_DIR := src
LIB_DIR := lib

# Generate paths for all object files
OBJS := $(patsubst %.c,%.o, $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/**/*.c))

# Compiler settings
CC := gcc

# Compiler and Linker flags Settings:
# 	-std=c11: Use the C11 standard
# 	-Wall: Enable all warnings
# 	-Wextra: Enable extra warnings
# 	-lm: Link to libm
CFLAGS := -std=c11 -Wall -Wextra
LDFLAGS := -lm

ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -Oz
endif

# Targets

# Build executable
swanc: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

# Build object files and third-party libraries
$(OBJS):
	@$(CC) $(CFLAGS) -o $@ -c $*.c

clean:
	@rm -r $(SRC_DIR)/*.o

watch:
	 watchexec "make -B && echo "" && ./swanc example/fibonacci.sn"

.PHONY: clean swanc watch
