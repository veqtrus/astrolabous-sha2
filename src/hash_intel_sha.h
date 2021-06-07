#ifndef ASTROLABOUS_HASH_INTEL_SHA_H
#define ASTROLABOUS_HASH_INTEL_SHA_H

#include <stdint.h>
#include <stdbool.h>

void astrolabous_hash_intel_sha(uint32_t *buf, uint64_t n_iter);

bool astrolabous_intel_sha_avail(void);

#endif
