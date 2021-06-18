#ifndef ASTROLABOUS_H
#define ASTROLABOUS_H

#include <stdint.h>

#define ASTROLABOUS_KEY_SIZE 32

#ifdef __cplusplus
#define ASTROLABOUS_EXTERN extern "C"
#else
#define ASTROLABOUS_EXTERN extern
#endif

#if defined(__cplusplus) || __STDC_VERSION__ >= 199901L
#define ASTROLABOUS_INLINE static inline
#else
#define ASTROLABOUS_INLINE static
#endif

ASTROLABOUS_INLINE uint32_t astrolabous_cipher_size(uint32_t n_chain)
{
	return (n_chain + 1) * ASTROLABOUS_KEY_SIZE;
}

ASTROLABOUS_INLINE uint32_t astrolabous_witness_size(uint32_t n_chain)
{
	return n_chain * ASTROLABOUS_KEY_SIZE;
}

ASTROLABOUS_EXTERN int astrolabous_max_threads(void);

ASTROLABOUS_EXTERN char *astrolabous_encrypt_key(
		const uint8_t *key,
		uint8_t *ckey,
		uint8_t *wkey,
		uint32_t n_chain,
		uint64_t n_iter);

ASTROLABOUS_EXTERN char *astrolabous_decrypt_key(
		uint8_t *key,
		const uint8_t *ckey,
		const uint8_t *wkey,
		uint32_t n_chain,
		uint64_t n_iter);

ASTROLABOUS_EXTERN char *astrolabous_recover_key(
		uint8_t *key,
		const uint8_t *ckey,
		uint8_t *wkey,
		uint32_t n_chain,
		uint64_t n_iter);

#endif
