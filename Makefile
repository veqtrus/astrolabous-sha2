WARNINGS := -Wall
INCLUDES := -Iinclude/
CFLAGS := -std=c99 -O2 $(INCLUDES) $(WARNINGS)
$(CC) ?= gcc

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.out)

CFLAGS_INTEL_SHA := $(CFLAGS)
ifeq ($(ASTROLABOUS_INTEL_SHA),1)
CFLAGS_INTEL_SHA += -mavx -msha -DASTROLABOUS_INTEL_SHA=1
endif

all: test

test: $(TEST_OBJECTS)

clean:
	rm -f src/*.o test/*.out

src/hash_intel_sha.o: src/hash_intel_sha.c
	$(CC) -o $@ -c $< $(CFLAGS_INTEL_SHA)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.out: %.c $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

src/cipher_size.c: include/astrolabous/astrolabous.h
src/hash_generic.c: src/hash_generic.h src/sha2.h
src/hash_intel_sha.c: src/hash_intel_sha.h
src/recover_key.c: include/astrolabous/astrolabous.h src/hash_generic.h src/hash_intel_sha.h src/words.h
src/words.c: src/words.h
