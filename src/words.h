#ifndef ASTROLABOUS_WORDS_H
#define ASTROLABOUS_WORDS_H

#include <stdint.h>

void astrolabous_words_from_bytes(uint32_t *w, const uint8_t *b, uint32_t n_words);

void astrolabous_words_to_bytes(const uint32_t *w, uint8_t *b, uint32_t n_words);

#endif
