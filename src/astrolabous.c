#include "astrolabous/astrolabous.h"

#include <string.h>
#include <stdlib.h>

#include "secure_random.h"
#include "parallel_hash.h"
#include "hash.h"
#include "words.h"

char *astrolabous_encrypt_key(
		const uint8_t *key,
		uint8_t *ckey,
		uint8_t *wkey,
		uint32_t n_chain,
		uint64_t n_iter)
{
	char *err_str;
	uint32_t *seeds;
	uint32_t *hashes;
	size_t i, size;
	astrolabous_parallel_hash_fn hash;
	if (n_chain == 0) {
		err_str = malloc(32);
		strcpy(err_str, "n_chain must be at least 1");
		return err_str;
	}
	if (n_iter == 0) {
		err_str = malloc(32);
		strcpy(err_str, "n_iter must be at least 1");
		return err_str;
	}
	size = n_chain * ASTROLABOUS_KEY_SIZE;
	seeds = malloc(size + ASTROLABOUS_KEY_SIZE);
	hashes = malloc(size);
	err_str = astrolabous_secure_random((uint8_t *) seeds, size);
	if (err_str) goto cleanup;
	memcpy(hashes, seeds, size);
	hash = astrolabous_parallel_hash_impl();
	err_str = hash(hashes, n_chain, n_iter);
	if (err_str) goto cleanup;
	size = n_chain * 8;
	astrolabous_words_from_bytes(seeds + size, key, 8);
	for (i = 0; i < size; ++i)
		seeds[i + 8] ^= hashes[i];
	if (ckey)
		astrolabous_words_to_bytes(seeds, ckey, size + 8);
	if (wkey)
		astrolabous_words_to_bytes(hashes, wkey, size);
cleanup:
	free(seeds);
	free(hashes);
	return err_str;
}

char *astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint8_t *wkey,
		uint32_t n_chain,
		uint64_t n_iter)
{
	uint32_t buf[8];
	uint32_t pad[8];
	char *err_str;
	astrolabous_hash_fn hash;
	int i;
	if (n_chain == 0) {
		err_str = malloc(32);
		strcpy(err_str, "n_chain must be at least 1");
		return err_str;
	}
	if (n_iter == 0) {
		err_str = malloc(32);
		strcpy(err_str, "n_iter must be at least 1");
		return err_str;
	}
	hash = astrolabous_hash_impl();
	astrolabous_words_from_bytes(buf, ckey, 8);
	do {
		hash(buf, n_iter);
		if (wkey) {
			astrolabous_words_to_bytes(buf, wkey, 8);
			wkey += ASTROLABOUS_KEY_SIZE;
		}
		ckey += ASTROLABOUS_KEY_SIZE;
		astrolabous_words_from_bytes(pad, ckey, 8);
		for (i = 0; i < 8; ++i)
			buf[i] ^= pad[i];
	} while (--n_chain);
	if (key)
		astrolabous_words_to_bytes(buf, key, 8);
	return NULL;
}
