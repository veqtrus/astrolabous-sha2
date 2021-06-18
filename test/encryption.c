#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "astrolabous/astrolabous.h"

int main(int argc, char **argv)
{
	const uint8_t key[ASTROLABOUS_KEY_SIZE] = { 1, 2, 3 };
	uint8_t decr_key[ASTROLABOUS_KEY_SIZE];
	char *err_str = NULL;
	int ret = 0;
	uint8_t *cipher;
	uint8_t *witness;
	uint32_t n_chain = 1024;
	uint64_t n_iter = 1024;
	uint64_t n_hashes;
	clock_t start;
	double duration;
	if (argc == 3) {
		n_chain = atoi(argv[1]);
		n_iter = 1;
		n_iter <<= atoi(argv[2]);
	}
	n_hashes = n_iter * n_chain;
	cipher = malloc(astrolabous_cipher_size(n_chain));
	witness = malloc(astrolabous_witness_size(n_chain));
	start = clock();
	err_str = astrolabous_encrypt_key(key, cipher, witness, n_chain, n_iter);
	duration = (clock() - start) / (double) CLOCKS_PER_SEC;
	if (err_str) {
		fprintf(stderr, "%s\n", err_str);
		ret = 1;
		goto exit;
	}
	printf("Encrypted in %f s (%f MH/s)\n", duration, n_hashes * 1e-6 / duration);
	start = clock();
	astrolabous_recover_key(decr_key, cipher, NULL, n_chain, n_iter);
	duration = (clock() - start) / (double) CLOCKS_PER_SEC;
	if (memcmp(key, decr_key, ASTROLABOUS_KEY_SIZE)) {
		fprintf(stderr, "Keys do not match\n");
		ret = 2;
		goto exit;
	}
	printf("Recovered in %f s (%f MH/s)\n", duration, n_hashes * 1e-6 / duration);
exit:
	free(err_str);
	free(cipher);
	free(witness);
	return ret;
}
