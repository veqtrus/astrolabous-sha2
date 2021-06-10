__kernel void parallel_hash(__global unsigned int *buf, unsigned long n_iter)
{
	unsigned int hash[8];
	int i;
	buf += get_global_id(0) * 8;
	for (i = 0; i < 8; ++i)
		hash[i] = buf[i];
	do sha2(hash); while (--n_iter);
	for (i = 0; i < 8; ++i)
		buf[i] = hash[i];
}
