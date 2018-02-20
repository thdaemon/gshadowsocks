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

char *workdir;
char *srvdir;
char *conff;

void path_init()
{
	const char *confdir = g_get_user_config_dir();
	const char *suffix = "/gshadowsocks/";
	const char *sev_suffix = "servers/";
	const char *conf_suffix = "conf";

	size_t len = strlen(confdir) + strlen(suffix) + 1;
	workdir = malloc(len);
	snprintf(workdir, len, "%s%s", confdir, suffix);

	size_t lenx = len + strlen(sev_suffix);
	srvdir = malloc(lenx);
	snprintf(srvdir, lenx, "%s%s", workdir, sev_suffix);

	lenx = len + strlen(conf_suffix);
	conff = malloc(lenx);
	snprintf(conff, lenx, "%s%s", workdir, conf_suffix);
}
