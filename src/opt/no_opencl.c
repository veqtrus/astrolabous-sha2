#include "parallel_hash_opencl.h"

#include <stddef.h>

char *astrolabous_parallel_hash_opencl(
		uint32_t *buf, uint32_t n_chain, uint64_t n_iter)
{
	(void) buf; (void) n_chain; (void) n_iter;
	return NULL;
}

int astrolabous_opencl_max_threads(void)
{
	return 0;
}
