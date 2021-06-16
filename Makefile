WARNINGS ?= -Wall
INCLUDES ?= -Iinclude/
CFLAGS ?= -std=c99 -O2 $(INCLUDES) $(WARNINGS)
CC ?= gcc

ASTROLABOUS_OPT := $(subst \,, ,$(ASTROLABOUS_OPT))

ifneq (,$(findstring intel_sha,$(ASTROLABOUS_OPT)))
SOURCES += src/opt/hash_intel_sha.c
else
SOURCES += src/opt/no_intel_sha.c
endif

ifneq (,$(findstring opencl,$(ASTROLABOUS_OPT)))
LDFLAGS += -lOpenCL
SOURCES += src/opt/parallel_hash_opencl.c src/opt/parallel_hash_source.tmp.c
else
SOURCES += src/opt/no_opencl.c
endif

SOURCES += $(wildcard src/*.c) src/opt/hash_generic.c
OBJECTS = $(sort $(SOURCES:.c=.o))

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.out)

all: libastrolabous.a test

test: $(TEST_OBJECTS)

clean:
	rm -f *.a test/*.out src/*.o src/opt/*.o src/opt/*.tmp.c

libastrolabous.a: $(OBJECTS)
	ar rcs $@ $^

src/opt/parallel_hash_source.tmp.c: src/opt/sha2.h src/opt/parallel_hash.cl
	python3 util/pack_sources.py astrolabous_parallel_hash_source $@ $^

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

src/opt/hash_intel_sha.o: src/opt/hash_intel_sha.c
	$(CC) $< -o $@ -c $(CFLAGS) -mavx -msha

%.out: %.c libastrolabous.a
	$(CC) $^ -o $@ $(LDFLAGS) $(CFLAGS)

src/opt/hash_generic.c: src/opt/hash_generic.h src/opt/sha2.h
src/opt/hash_intel_sha.c: src/opt/hash_intel_sha.h
src/opt/no_intel_sha.c: src/opt/hash_intel_sha.h
src/opt/parallel_hash_opencl.c: src/opt/parallel_hash_opencl.h
src/opt/no_opencl.c: src/opt/parallel_hash_opencl.h
src/astrolabous.c: include/astrolabous/astrolabous.h
src/hash.c: src/hash.h src/opt/hash_generic.h src/opt/hash_intel_sha.h
src/parallel_hash.c: src/parallel_hash.h src/opt/parallel_hash_opencl.h include/astrolabous/astrolabous.h
src/words.c: src/words.h
