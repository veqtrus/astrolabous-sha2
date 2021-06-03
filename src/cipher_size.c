#include "astrolabous/astrolabous.h"

uint32_t astrolabous_cipher_size(uint32_t n_hash)
{
	return (n_hash + 1) * ASTROLABOUS_KEY_SIZE;
}
