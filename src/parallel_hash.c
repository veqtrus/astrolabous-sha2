#include "parallel_hash.h"

#include <stddef.h>

#include "hash.h"
#include "opt/parallel_hash_opencl.h"
#include "astrolabous/astrolabous.h"

static char *parallel_hash_fallback(
		uint32_t *buf, uint32_t n_chain, uint64_t n_iter)
{
	uint32_t *buf_end;
	astrolabous_hash_fn hash;
	hash = astrolabous_hash_impl();
	for (buf_end = buf + n_chain * 8; buf < buf_end; buf += 8)
		hash(buf, n_iter);
	return NULL;
}

astrolabous_parallel_hash_fn astrolabous_parallel_hash_impl(void)
{
	if (astrolabous_opencl_max_threads() > 0)
		return astrolabous_parallel_hash_opencl;
	else
		return parallel_hash_fallback;
}

int astrolabous_max_threads(void)
{
	int ret;
	ret = astrolabous_opencl_max_threads();
	if (ret > 0) return ret;
	return 1;
}
