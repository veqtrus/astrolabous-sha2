#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*astrolabous_hash_fn)(uint32_t *buf, uint64_t n_iter);

astrolabous_hash_fn astrolabous_hash_impl(void);

typedef char *(*astrolabous_parallel_hash_fn)(
		uint32_t *buf, uint32_t n_hash, uint64_t n_iter);

astrolabous_parallel_hash_fn astrolabous_parallel_hash_impl(void);

int astrolabous_opencl_max_threads(void);

static int single_hash(uint64_t n_iter)
{
	uint32_t buf[8] = { 0 };
	astrolabous_hash_fn hash;
	hash = astrolabous_hash_impl();
	hash(buf, n_iter);
	printf("%08x\n", buf[0]);
	return 0;
}

static int parallel_hash(uint32_t n_hash, uint64_t n_iter)
{
	char *err;
	uint32_t *buf;
	astrolabous_parallel_hash_fn hash;
	buf = calloc(n_hash, 32);
	hash = astrolabous_parallel_hash_impl();
	err = hash(buf, n_hash, n_iter);
	if (err) {
		fprintf(stderr, "%s\n", err);
		free(err);
		return 2;
	}
	printf("%08x\n", buf[0]);
	free(buf);
	return 0;
}

int main(int argc, char **argv)
{
	uint32_t n_hash = 1;
	uint64_t n_iter = 1;
	if (argc > 1)
		n_iter <<= atoi(argv[1]);
	if (argc > 2)
		n_hash = atoi(argv[2]);
	if (n_hash > 1) {
		printf("Max OpenCL threads: %d\n", astrolabous_opencl_max_threads());
		return parallel_hash(n_hash, n_iter);
	} else if (n_hash == 1) {
		return single_hash(n_iter);
	} else {
		fprintf(stderr, "n_hash must be at least 1\n");
		return 1;
	}
}
