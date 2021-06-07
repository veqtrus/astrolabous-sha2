#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*astrolabous_hash_fn)(uint32_t *buf, uint64_t n_iter);

astrolabous_hash_fn astrolabous_hash_impl(void);

int main(int argc, char **argv)
{
	uint32_t buf[8] = { 0 };
	uint64_t n_iter = 1;
	astrolabous_hash_fn hash;
	if (argc > 1)
		n_iter <<= atoi(argv[1]);
	hash = astrolabous_hash_impl();
	hash(buf, n_iter);
	printf("%08x\n", buf[0]);
	return 0;
}
