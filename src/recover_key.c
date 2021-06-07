#include "astrolabous/astrolabous.h"

#include "words.h"
#include "hash_generic.h"
#include "hash_intel_sha.h"

void astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint32_t n_hash,
		uint64_t n_iter)
{
	uint32_t buf[8];
	uint32_t pad[8];
	void (*hash)(uint32_t *, uint64_t);
	int i;
	if (n_hash == 0 || n_iter == 0) return;
	if (astrolabous_intel_sha_available())
		hash = astrolabous_hash_intel_sha;
	else
		hash = astrolabous_hash_generic;
	astrolabous_words_from_bytes(buf, ckey, 8);
	do {
		hash(buf, n_iter);
		ckey += ASTROLABOUS_KEY_SIZE;
		astrolabous_words_from_bytes(pad, ckey, 8);
		for (i = 0; i < 8; ++i)
			buf[i] ^= pad[i];
	} while (--n_hash);
	astrolabous_words_to_bytes(buf, key, 8);
}
