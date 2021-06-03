#include <stdint.h>
#include <stdio.h>

void astrolabous_hash(uint8_t *buf, uint64_t n_iter);

int main()
{
	uint8_t hash[32] = { 0 };
	astrolabous_hash(hash, 2);
	printf("%02x%02x%02x%02x\n", hash[0], hash[1], hash[2], hash[3]);
	return 0;
}
