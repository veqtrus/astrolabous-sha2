#ifndef ASTROLABOUS_DECRYPT_H
#define ASTROLABOUS_DECRYPT_H

#include "astrolabous.h"

ASTROLABOUS_EXTERN void astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint64_t n_hash,
		uint64_t n_iter);

#endif
