CC          := clang
FORMAT      := clang-format
CFLAGS	    := -Wall -Wextra -Wpedantic -Wconversion -Werror -std=c23 -O3 -Iinclude
DEBUG_FLAGS := -g -DDEBUG -O0

UNAME := $(shell uname -s)

# select platform-specific settings
ifeq ($(UNAME), Darwin)
	SANITIZER := -fsanitize=address,undefined
    LD_FLAGS := -framework OpenCL
else ifeq ($(UNAME), Linux)
	SANITIZER := -fsanitize=address,undefined,leak
    LD_FLAGS := -lOpenCL
else
	$(error Unsupported platform: $(UNAME))
endif

DEB_FLAGS += $(SANITIZER)

SRCS := $(addprefix src/, vm.c program.c)
INCLUDES := $(wildcard include/**/*.h)

MAIN_OBJS := $(SRCS:src/%.c=build/main/%.o) build/main/main.o
TEST_OBJS := $(SRCS:src/%.c=build/test/%.o) build/test/test.o

build/main/%.o: src/%.c $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

build/test/%.o: src/%.c $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -DWINTER_ENABLED -c -o $@ $<

test: $(TEST_OBJS)
	$(CC) $(SANITIZER) $^ -o $@

main: $(MAIN_OBJS)
	$(CC) $^ -o $@

clean:
	rm -rf build
	rm -f lib.* test

format:
	find . -name '*.c' -o -name '*.h' | xargs $(FORMAT) -i

.PHONY: clean format
