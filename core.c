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

#include <gtk/gtk.h>

#include "paths.h"
#include "conf.h"

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

int core_server_add(const char *srvname, const char *srvaddr,
		    const char *srvport, const char *localport,
		    const char *passwd, const char *method)
{
	char path[2048];
	GKeyFile *kfile;

	snprintf(path, 2048, "%s/%s", srvdir, srvname);

	kfile = conf_open(path);

	conf_set_string(kfile, SERVER_ADDR, srvaddr);
	conf_set_string(kfile, SERVER_PORT, srvport);
	conf_set_string(kfile, SERVER_LOCAL_PORT, localport);
	conf_set_string(kfile, SERVER_PASSWD, passwd);
	conf_set_string(kfile, SERVER_METHOD, method);

	return ((conf_close(kfile, path) == TRUE) ? 0 : -1);
}

int core_server_exists(const char *srvname)
{
	int fd, ret;

	if ((fd = open(srvdir, O_DIRECTORY)) < 0) {
		return ((errno == ENOENT) ? 0 : -1);
	}

	ret = faccessat(fd, srvname, F_OK, 0);
	close(fd);

	return ret;
}
