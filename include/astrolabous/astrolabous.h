#ifndef ASTROLABOUS_H
#define ASTROLABOUS_H

#include <stdint.h>

#define ASTROLABOUS_KEY_SIZE 32

#ifdef __cplusplus
#define ASTROLABOUS_EXTERN extern "C"
#else
#define ASTROLABOUS_EXTERN extern
#endif

ASTROLABOUS_EXTERN uint32_t astrolabous_cipher_size(uint32_t n_hash);

ASTROLABOUS_EXTERN int astrolabous_max_threads(void);

ASTROLABOUS_EXTERN void astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint32_t n_hash,
		uint64_t n_iter);

ASTROLABOUS_EXTERN void astrolabous_decrypt_key(
		uint8_t *key,
		const uint8_t *wkey,
		uint32_t n_hash,
		uint64_t n_iter);

#endif
