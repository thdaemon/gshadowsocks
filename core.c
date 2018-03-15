#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE

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

GSubprocess *ssproc = NULL;

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

static struct json_row {
	const char *name;
	const char *value;
	char quotes;
} rows[] = {
	{ "server", NULL, 1 },
	{ "server_port", NULL, 0 },
	{ "local_address", "127.0.0.1", 1 },
	{ "local_port", NULL, 0 },
	{ "password", NULL, 1 },
	{ "method", NULL, 1 },
	{ "timeout", "300", 0 },
	{ "fast_open", "false", 0 }
};

static char *json_gen()
{
	size_t len = 3;
	size_t n;
	char *buf;

	n = sizeof(rows)/sizeof(rows[0]);

	for (int i = 0; i < n; i++) {
		len = len + strlen(rows[i].name) + 6
		      + strlen(rows[i].value)
		      + ((rows[i].quotes) ? 2 : 0);
	}

	if ((buf = malloc(len)) == NULL) {
		return NULL;
	} else {
		buf[0] = '\0';
		buf = strcat(buf, "{\n");
		for (int i = 0; i < n; i++) {
			buf = strcat(buf, "\t\"");
			buf = strcat(buf, rows[i].name);
			buf = strcat(buf, "\":");
			if (rows[i].quotes)
				buf = strcat(buf, "\"");
			buf = strcat(buf, rows[i].value);
			if (rows[i].quotes)
				buf = strcat(buf, "\"");
			buf = strcat(buf, (i+1 == n) ? "\n" : ",\n");
		}
		buf = strcat(buf, "}");
		return buf;
	}
}

int core_sconf_gen(const char *srvname)
{
	char path[2048];
	GKeyFile *kfile;
	char *json;
	gboolean ret;

	snprintf(path, 2048, "%s/%s", srvdir, srvname);

	kfile = conf_open(path);

	rows[0].value = conf_get_string(kfile, SERVER_ADDR);
	rows[1].value = conf_get_string(kfile, SERVER_PORT);
	rows[3].value = conf_get_string(kfile, SERVER_LOCAL_PORT);
	rows[4].value = conf_get_string(kfile, SERVER_PASSWD);
	rows[5].value = conf_get_string(kfile, SERVER_METHOD);

	conf_close(kfile, NULL);

	snprintf(path, 2048, "%s/ssconf.gen", workdir);

	json = json_gen();
	ret = g_file_set_contents(path, json, -1, NULL);
	free(json);

	return ((ret == TRUE) ? 0 : -1);
}

int core_start_sslocal()
{
	char path[2048];

	snprintf(path, 2048, "%sssconf.gen", workdir);

	if (ssproc == NULL) {
		ssproc = g_subprocess_new(G_SUBPROCESS_FLAGS_NONE, NULL,
					  "ss-local", "-c", path, NULL);
		return ((ssproc == NULL) ? -1 : 0);
	} else {
		return -1;
	}
}

void core_stop_sslocal()
{
	if (ssproc) {
		g_subprocess_send_signal(ssproc, SIGTERM);
		usleep(100);
		g_subprocess_force_exit(ssproc);
		g_object_unref(ssproc);
		ssproc = NULL;
	}
}

int core_pac_update(const char *srvname)
{
	int ret = 0;
	char path[2048];
	GKeyFile *kfile;
	const char *port;
	GSubprocess *subp;
	GCancellable *cable;

	snprintf(path, 2048, "%s/%s", srvdir, srvname);

	kfile = conf_open(path);
	port = conf_get_string(kfile, SERVER_LOCAL_PORT);

	cable = g_cancellable_new();
	subp = g_subprocess_new(G_SUBPROCESS_FLAGS_NONE, NULL,
				UPDATE_PAC_PATH, CONFIG_PREFIX, port, NULL);

	g_subprocess_wait(subp, cable, NULL);

	if (!g_subprocess_get_if_exited(subp)) {
		ret = -1;
		goto fail;
	}

	if (g_subprocess_get_exit_status(subp) != 0) {
		ret = -1;
		goto fail;
	}

fail:
	conf_close(kfile, NULL);
	g_object_unref(subp);
	g_object_unref(cable);
	return ret;
}

int core_proxy_helper(int opcode, const char *srvname)
{
	int ret = 0;
	char _opcode[25];
	char path[2048];
	GKeyFile *kfile;
	const char *port;
	GSubprocess *subp;
	GCancellable *cable;

	snprintf(path, 2048, "%s/%s", srvdir, srvname);

	kfile = conf_open(path);
	port = conf_get_string(kfile, SERVER_LOCAL_PORT);

	snprintf(_opcode, 25, "%d", opcode);

	cable = g_cancellable_new();
	subp = g_subprocess_new(G_SUBPROCESS_FLAGS_NONE, NULL,
				PROXY_HELPER_PATH, _opcode, port, NULL);

	g_subprocess_wait(subp, cable, NULL);

	if (!g_subprocess_get_if_exited(subp)) {
		ret = -1;
		goto fail;
	}

	if (g_subprocess_get_exit_status(subp) != 0) {
		ret = -1;
		goto fail;
	}

fail:
	conf_close(kfile, NULL);
	g_object_unref(subp);
	g_object_unref(cable);
	return ret;
}
