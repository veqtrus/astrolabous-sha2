#include "hash_generic.h"

#include "sha2.h"

void astrolabous_hash_generic(uint32_t *buf, uint64_t n_iter)
{
	do sha2(buf); while (--n_iter);
}
