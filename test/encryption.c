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
	if (argc == 3) {
		n_chain = atoi(argv[1]);
		n_iter = 1;
		n_iter <<= atoi(argv[2]);
	}
	cipher = malloc(astrolabous_cipher_size(n_chain));
	witness = malloc(astrolabous_witness_size(n_chain));
	err_str = astrolabous_encrypt_key(key, cipher, witness, n_chain, n_iter);
	if (err_str) {
		fprintf(stderr, "%s\n", err_str);
		ret = 1;
		goto exit;
	}
	puts("Encrypted");
	astrolabous_recover_key(decr_key, cipher, n_chain, n_iter);
	puts("Decrypted");
	if (memcmp(key, decr_key, ASTROLABOUS_KEY_SIZE)) {
		fprintf(stderr, "Keys do not match\n");
		ret = 2;
		goto exit;
	}
exit:
	free(err_str);
	free(cipher);
	free(witness);
	return ret;
}
