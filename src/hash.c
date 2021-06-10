#include "hash.h"

#include "opt/hash_generic.h"
#include "opt/hash_intel_sha.h"

astrolabous_hash_fn astrolabous_hash_impl(void)
{
	if (astrolabous_intel_sha_avail())
		return astrolabous_hash_intel_sha;
	else
		return astrolabous_hash_generic;
}
