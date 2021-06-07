#include "hash_intel_sha.h"

#if ASTROLABOUS_INTEL_SHA

#include <immintrin.h>

// Based on https://github.com/noloader/SHA-Intrinsics/blob/master/sha256-x86.c
static void sha2(uint32_t *buf)
{
	static const uint32_t padding[] = { 0x80000000UL, 0, 0, 0, 0, 0, 0, 256 };
	__m128i state0, state1, state0saved, state1saved;
	__m128i msg, tmp, msg0, msg1, msg2, msg3;
	
	state0 = _mm_set_epi32(
		0x6a09e667UL, // a
		0xbb67ae85UL, // b
		0x510e527fUL, // e
		0x9b05688cUL  // f
	);
	state1 = _mm_set_epi32(
		0x3c6ef372UL, // c
		0xa54ff53aUL, // d
		0x1f83d9abUL, // g
		0x5be0cd19UL  // h
	);
	
	/* Save current state */
	state0saved = state0, state1saved = state1;
	
	/* Rounds 0-3 */
	msg0 = _mm_loadu_si128((const __m128i*) (buf));
	msg = _mm_add_epi32(msg0, _mm_set_epi64x(0xE9B5DBA5B5C0FBCFULL, 0x71374491428A2F98ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	
	/* Rounds 4-7 */
	msg1 = _mm_loadu_si128((const __m128i*) (buf+4));
	msg = _mm_add_epi32(msg1, _mm_set_epi64x(0xAB1C5ED5923F82A4ULL, 0x59F111F13956C25BULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg0 = _mm_sha256msg1_epu32(msg0, msg1);
	
	/* Rounds 8-11 */
	msg2 = _mm_loadu_si128((const __m128i*) (padding));
	msg = _mm_add_epi32(msg2, _mm_set_epi64x(0x550C7DC3243185BEULL, 0x12835B01D807AA98ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg1 = _mm_sha256msg1_epu32(msg1, msg2);
	
	/* Rounds 12-15 */
	msg3 = _mm_loadu_si128((const __m128i*) (padding+4));
	msg = _mm_add_epi32(msg3, _mm_set_epi64x(0xC19BF1749BDC06A7ULL, 0x80DEB1FE72BE5D74ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg3, msg2, 4);
	msg0 = _mm_add_epi32(msg0, tmp);
	msg0 = _mm_sha256msg2_epu32(msg0, msg3);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg2 = _mm_sha256msg1_epu32(msg2, msg3);
	
	/* Rounds 16-19 */
	msg = _mm_add_epi32(msg0, _mm_set_epi64x(0x240CA1CC0FC19DC6ULL, 0xEFBE4786E49B69C1ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg0, msg3, 4);
	msg1 = _mm_add_epi32(msg1, tmp);
	msg1 = _mm_sha256msg2_epu32(msg1, msg0);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg3 = _mm_sha256msg1_epu32(msg3, msg0);
	
	/* Rounds 20-23 */
	msg = _mm_add_epi32(msg1, _mm_set_epi64x(0x76F988DA5CB0A9DCULL, 0x4A7484AA2DE92C6FULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg1, msg0, 4);
	msg2 = _mm_add_epi32(msg2, tmp);
	msg2 = _mm_sha256msg2_epu32(msg2, msg1);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg0 = _mm_sha256msg1_epu32(msg0, msg1);
	
	/* Rounds 24-27 */
	msg = _mm_add_epi32(msg2, _mm_set_epi64x(0xBF597FC7B00327C8ULL, 0xA831C66D983E5152ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg2, msg1, 4);
	msg3 = _mm_add_epi32(msg3, tmp);
	msg3 = _mm_sha256msg2_epu32(msg3, msg2);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg1 = _mm_sha256msg1_epu32(msg1, msg2);
	
	/* Rounds 28-31 */
	msg = _mm_add_epi32(msg3, _mm_set_epi64x(0x1429296706CA6351ULL,  0xD5A79147C6E00BF3ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg3, msg2, 4);
	msg0 = _mm_add_epi32(msg0, tmp);
	msg0 = _mm_sha256msg2_epu32(msg0, msg3);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg2 = _mm_sha256msg1_epu32(msg2, msg3);
	
	/* Rounds 32-35 */
	msg = _mm_add_epi32(msg0, _mm_set_epi64x(0x53380D134D2C6DFCULL, 0x2E1B213827B70A85ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg0, msg3, 4);
	msg1 = _mm_add_epi32(msg1, tmp);
	msg1 = _mm_sha256msg2_epu32(msg1, msg0);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg3 = _mm_sha256msg1_epu32(msg3, msg0);
	
	/* Rounds 36-39 */
	msg = _mm_add_epi32(msg1, _mm_set_epi64x(0x92722C8581C2C92EULL, 0x766A0ABB650A7354ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg1, msg0, 4);
	msg2 = _mm_add_epi32(msg2, tmp);
	msg2 = _mm_sha256msg2_epu32(msg2, msg1);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg0 = _mm_sha256msg1_epu32(msg0, msg1);
	
	/* Rounds 40-43 */
	msg = _mm_add_epi32(msg2, _mm_set_epi64x(0xC76C51A3C24B8B70ULL, 0xA81A664BA2BFE8A1ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg2, msg1, 4);
	msg3 = _mm_add_epi32(msg3, tmp);
	msg3 = _mm_sha256msg2_epu32(msg3, msg2);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg1 = _mm_sha256msg1_epu32(msg1, msg2);
	
	/* Rounds 44-47 */
	msg = _mm_add_epi32(msg3, _mm_set_epi64x(0x106AA070F40E3585ULL, 0xD6990624D192E819ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg3, msg2, 4);
	msg0 = _mm_add_epi32(msg0, tmp);
	msg0 = _mm_sha256msg2_epu32(msg0, msg3);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg2 = _mm_sha256msg1_epu32(msg2, msg3);
	
	/* Rounds 48-51 */
	msg = _mm_add_epi32(msg0, _mm_set_epi64x(0x34B0BCB52748774CULL, 0x1E376C0819A4C116ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg0, msg3, 4);
	msg1 = _mm_add_epi32(msg1, tmp);
	msg1 = _mm_sha256msg2_epu32(msg1, msg0);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	msg3 = _mm_sha256msg1_epu32(msg3, msg0);
	
	/* Rounds 52-55 */
	msg = _mm_add_epi32(msg1, _mm_set_epi64x(0x682E6FF35B9CCA4FULL, 0x4ED8AA4A391C0CB3ULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg1, msg0, 4);
	msg2 = _mm_add_epi32(msg2, tmp);
	msg2 = _mm_sha256msg2_epu32(msg2, msg1);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	
	/* Rounds 56-59 */
	msg = _mm_add_epi32(msg2, _mm_set_epi64x(0x8CC7020884C87814ULL, 0x78A5636F748F82EEULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	tmp = _mm_alignr_epi8(msg2, msg1, 4);
	msg3 = _mm_add_epi32(msg3, tmp);
	msg3 = _mm_sha256msg2_epu32(msg3, msg2);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	
	/* Rounds 60-63 */
	msg = _mm_add_epi32(msg3, _mm_set_epi64x(0xC67178F2BEF9A3F7ULL, 0xA4506CEB90BEFFFAULL));
	state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
	msg = _mm_shuffle_epi32(msg, 0x0E);
	state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
	
	/* Combine state  */
	state0 = _mm_add_epi32(state0, state0saved);
	state1 = _mm_add_epi32(state1, state1saved);
	
	tmp = _mm_shuffle_epi32(state0, 0x1B);       /* FEBA */
	state1 = _mm_shuffle_epi32(state1, 0xB1);    /* DCHG */
	state0 = _mm_blend_epi16(tmp, state1, 0xF0); /* DCBA */
	state1 = _mm_alignr_epi8(state1, tmp, 8);    /* ABEF */
	
	/* Save state */
	_mm_storeu_si128((__m128i*) &buf[0], state0);
	_mm_storeu_si128((__m128i*) &buf[4], state1);
}

void astrolabous_hash_intel_sha(uint32_t *buf, uint64_t n_iter)
{
	do sha2(buf); while (--n_iter);
}

bool astrolabous_intel_sha_available(void)
{
	int a, b, c, d;
	a = 7;
	c = 0;
	__asm volatile (
		"cpuid"
		: "=a"(a), "=b"(b), "=c"(c), "=d"(d)
		: "a"(a), "c"(c)
	);
	return ((b >> 29) & 1);
}

#else

void astrolabous_hash_intel_sha(uint32_t *buf, uint64_t n_iter)
{
	(void) buf; (void) n_iter;
}

bool astrolabous_intel_sha_available(void)
{
	return false;
}

#endif
