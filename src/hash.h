#ifndef ASTROLABOUS_HASH_H
#define ASTROLABOUS_HASH_H

#include <stdint.h>

typedef void (*astrolabous_hash_fn)(uint32_t *buf, uint64_t n_iter);

astrolabous_hash_fn astrolabous_hash_impl(void);

#endif
