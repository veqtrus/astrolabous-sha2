#include "words.h"

static uint32_t get_be32(const uint8_t *buf)
{
	return (((((((uint32_t) buf[0]) << 8)
		| buf[1]) << 8)
			| buf[2]) << 8)
				| buf[3];
}

static void put_be32(uint8_t *buf, uint32_t val)
{
	buf[3] = val;
	val >>= 8;
	buf[2] = val;
	val >>= 8;
	buf[1] = val;
	val >>= 8;
	buf[0] = val;
}

void astrolabous_words_from_bytes(uint32_t *w, const uint8_t *b, uint32_t n_words)
{
	do {
		*w++ = get_be32(b);
		b += sizeof(*w);
	} while (--n_words);
}

void astrolabous_words_to_bytes(const uint32_t *w, uint8_t *b, uint32_t n_words)
{
	do {
		put_be32(b, *w++);
		b += sizeof(*w);
	} while (--n_words);
}
