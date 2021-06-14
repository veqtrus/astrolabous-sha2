#ifndef ASTROLABOUS_PARALLEL_HASH_H
#define ASTROLABOUS_PARALLEL_HASH_H

#include <stdint.h>

typedef char *(*astrolabous_parallel_hash_fn)(
		uint32_t *buf, uint32_t n_chain, uint64_t n_iter);

astrolabous_parallel_hash_fn astrolabous_parallel_hash_impl(void);

#endif
