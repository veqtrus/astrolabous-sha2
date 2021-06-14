#include "astrolabous/astrolabous.h"

uint32_t astrolabous_cipher_size(uint32_t n_chain)
{
	return (n_chain + 1) * ASTROLABOUS_KEY_SIZE;
}
