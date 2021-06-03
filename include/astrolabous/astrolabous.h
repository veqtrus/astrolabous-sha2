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

#endif
