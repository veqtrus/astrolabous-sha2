#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void astrolabous_hash_generic(uint32_t *buf, uint64_t n_iter);

void astrolabous_hash_intel_sha(uint32_t *buf, uint64_t n_iter);

bool astrolabous_intel_sha_available(void);

int main(int argc, char **argv)
{
	uint32_t buf[8] = { 0 };
	void (*hash)(uint32_t *, uint64_t);
	uint64_t n_iter = 1;
	if (argc > 1)
		n_iter <<= atoi(argv[1]);
	if (astrolabous_intel_sha_available())
		hash = astrolabous_hash_intel_sha;
	else
		hash = astrolabous_hash_generic;
	hash(buf, n_iter);
	printf("%08x\n", buf[0]);
	return 0;
}
