WARNINGS := -Wall
INCLUDES := -Iinclude/
CFLAGS := -std=c99 -O2 -mavx -msha $(INCLUDES) $(WARNINGS)
$(CC) ?= gcc

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.out)

all: test

test: $(TEST_OBJECTS)

clean:
	rm -f src/*.o test/*.out

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.out: %.c $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)
