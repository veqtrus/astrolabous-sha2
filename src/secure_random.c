#include "secure_random.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

char *astrolabous_secure_random(uint8_t *buf, uint32_t len)
{
	char *err_str;
	int fd;
	ssize_t ret;
	if (len == 0) return NULL;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		err_str = malloc(48);
		snprintf(err_str, 48, "Error %d while opening /dev/urandom", errno);
		return err_str;
	}
	do {
		ret = read(fd, buf, len);
		if (ret <= 0) {
			err_str = malloc(48);
			if (ret == 0)
				strcpy(err_str, "End Of File while reading /dev/urandom");
			else
				snprintf(err_str, 48, "Error %d while reading /dev/urandom", errno);
			close(fd);
			return err_str;
		}
		buf += ret;
		len -= ret;
	} while (len);
	close(fd);
	return NULL;
}
