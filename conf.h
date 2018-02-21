#ifndef __CONF_H_
#define __CONF_H_

#define CONF_SS_CONN "ssconn"
#define CONF_SP_SET "sysproxy"
#define CONF_PROXY_MODE "proxymode"

#define CONF_DEFAULT_SERVER "default-server"

#define PROXY_MODE_PAC 0
#define PROXY_MODE_GLOBAL 1

GKeyFile *conf_open(const gchar *file);
int conf_get_int(GKeyFile *kfile, const gchar *key);
void conf_set_int(GKeyFile *kfile, const gchar *key, gint value);
char *conf_get_string(GKeyFile *kfile, const gchar *key);
void conf_set_string(GKeyFile *kfile, const gchar *key, gchar *value);
gboolean conf_close(GKeyFile *kfile, const gchar *file);

#endif /* __CONF_H_ */
