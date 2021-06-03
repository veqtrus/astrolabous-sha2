#include "astrolabous/astrolabous.h"

#include "hash.h"

void astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint32_t n_hash,
		uint64_t n_iter)
{
	uint8_t hash[ASTROLABOUS_KEY_SIZE];
	int i;
	for (i = 0; i < sizeof(hash); ++i)
		hash[i] = ckey[i];
	do {
		astrolabous_hash(hash, n_iter);
		ckey += ASTROLABOUS_KEY_SIZE;
		for (i = 0; i < sizeof(hash); ++i)
			hash[i] ^= ckey[i];
	} while (--n_hash);
	for (i = 0; i < sizeof(hash); ++i)
		key[i] = hash[i];
}
