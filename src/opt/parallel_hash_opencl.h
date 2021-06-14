#ifndef ASTROLABOUS_PARALLEL_HASH_OPENCL_H
#define ASTROLABOUS_PARALLEL_HASH_OPENCL_H

#include <stdint.h>

char *astrolabous_parallel_hash_opencl(
		uint32_t *buf, uint32_t n_chain, uint64_t n_iter);

int astrolabous_opencl_max_threads(void);

#endif
