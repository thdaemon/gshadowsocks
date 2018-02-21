#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "paths.h"

int core_server_del(const char *srvname)
{
	int fd, ret;

	if ((fd = open(srvdir, O_DIRECTORY)) < 0) {
		return ((errno == ENOENT) ? 0 : -1);
	}

	ret = unlinkat(fd, srvname, 0);
	close(fd);

	return ret;
}
