CC      := clang
TARGET  := arena
SRCS    := arena.c
OBJS    := $(SRCS:.c=.o)
DEPS    := arena.h

CFLAGS  := -std=c11 \
            -Wall \
            -Wextra \
            -Wpedantic \
            -Wshadow \
            -Wcast-align \
            -Wcast-qual \
            -Wconversion \
            -Wdouble-promotion \
            -Wfloat-equal \
            -Wformat=2 \
            -Wmissing-prototypes \
            -Wnull-dereference \
            -Wpointer-arith \
            -Wredundant-decls \
            -Wstrict-prototypes \
            -Wundef \
            -Wuninitialized \
            -Wunreachable-code \
            -Wwrite-strings \
            -Werror


DEBUG_FLAGS   := -g3 -O0 -fsanitize=address,undefined
RELEASE_FLAGS := -O2 -DNDEBUG

# Default to debug
CFLAGS += $(DEBUG_FLAGS)

.PHONY: all release clean

all: $(TARGET)

release: CFLAGS := $(filter-out $(DEBUG_FLAGS),$(CFLAGS)) $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)