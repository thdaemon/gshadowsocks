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

#include "conf.h"

GKeyFile *conf_open(const gchar *file)
{
	GKeyFile *kfile = g_key_file_new();
	g_key_file_load_from_file(kfile, file, G_KEY_FILE_NONE, NULL);
	return kfile;
}

int conf_get_int(GKeyFile *kfile, const gchar *key)
{
	return g_key_file_get_integer(kfile, "conf", key, NULL);
}

void conf_set_int(GKeyFile *kfile, const gchar *key, gint value)
{
	g_key_file_set_integer(kfile, "conf", key, value);
}

char *conf_get_string(GKeyFile *kfile, const gchar *key)
{
	return g_key_file_get_value(kfile, "conf", key, NULL);
}

void conf_set_string(GKeyFile *kfile, const gchar *key, gchar *value)
{
	if (value != NULL)
		g_key_file_set_value(kfile, "conf", key, value);
}

gboolean conf_close(GKeyFile *kfile, const gchar *file)
{
	gboolean ret;

	ret = g_key_file_save_to_file(kfile, file, NULL);
	g_key_file_free(kfile);

	return ret;
}

