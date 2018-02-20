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
#include <libappindicator/app-indicator.h>

#include "paths.h"
#include "srvwin.h"
#include "conf.h"

static GtkWidget *defserver_menu;

static int proxy_mode;

static void mevt_servers(GtkMenuItem *menuitem, gpointer user_data)
{
	srvwin_show();
}

static void mevt_quit(GtkMenuItem *menuitem, gpointer user_data)
{
	gtk_main_quit();
}

static void mevt_proxy_mode(GtkMenuItem *menuitem, gpointer user_data)
{
	proxy_mode = (long)user_data;
}

static GtkWidget *add_menu_item(GtkWidget *menu, gchar *lable, gpointer callback)
{
	GtkWidget *item;

	item = gtk_menu_item_new_with_label(lable);

	if (callback)
		g_signal_connect(item, "activate", G_CALLBACK(callback), item);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	return item;
}

static GtkWidget *add_menu_check_item(GtkWidget *menu, gchar *lable, gpointer callback,
				      gboolean is_active)
{
	GtkWidget *item;

	item = gtk_check_menu_item_new_with_label(lable);

	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), is_active);

	if (callback)
		g_signal_connect(item, "activate", G_CALLBACK(callback), item);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	return item;
}

static GtkWidget *add_menu_radio_item(GtkWidget *menu, GSList **group, gchar *lable,
			       gpointer callback, gboolean is_active, gpointer udata)
{
	GtkWidget *item;

	item = gtk_radio_menu_item_new_with_label(*group, lable);

	if (*group == NULL) {
		GSList *g = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item));
		*group = g;
	}

	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), is_active);

	if (callback)
		g_signal_connect(item, "activate", G_CALLBACK(callback), udata);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	return item;
}

static void add_menu_separator(GtkWidget *menu)
{
	GtkWidget *item;
	item = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

void update_defserver_menu_list()
{
	int flag_have = 0;
	GSList *group = NULL;
	GtkWidget *submenu = gtk_menu_new();

	DIR* dir;
	struct dirent *dent;

	if ((dir = opendir(srvdir)) == NULL)
		return;

	while ((dent = readdir(dir)) != NULL) {
		if (dent->d_name[0] != '.') {
			add_menu_radio_item(submenu, &group, dent->d_name, NULL, FALSE, NULL);
			flag_have = 1;
		}
	}

	if (flag_have) {
		gtk_widget_set_sensitive(defserver_menu, TRUE);
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(defserver_menu), submenu);
	} else {
		gtk_widget_set_sensitive(defserver_menu, FALSE);
	}

	closedir(dir);
}

int main(int argc, char **argv)
{
	GKeyFile *kfile;
	AppIndicator *indicator;
	GtkWidget *menu, *submenu;
	GtkWidget *tmp;
	GSList *group = NULL;

	gtk_init(&argc, &argv);

	path_init();
	mkdir(workdir, 0750);
	mkdir(srvdir, 0750);

	kfile = conf_open(conff);
	proxy_mode = conf_get_int(kfile, CONF_PROXY_MODE);

	indicator = app_indicator_new("gshadowsock-tray-icon-id", "gshadowsocks",
				      APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

	app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
	app_indicator_set_icon(indicator, ICON_FILE_NORMAL);

	menu = gtk_menu_new();

	add_menu_check_item(menu, "Connect shadowsocks proxy", NULL, FALSE);
	add_menu_check_item(menu, "Set system proxy settings", NULL, FALSE);
	add_menu_separator(menu);
	tmp = add_menu_item(menu, "System proxy mode", NULL);
	submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(tmp), submenu);
	group = NULL;
	add_menu_radio_item(submenu, &group, "PAC mode", mevt_proxy_mode,
			    (proxy_mode == PROXY_MODE_PAC), (gpointer)0);
	add_menu_radio_item(submenu, &group, "Global", mevt_proxy_mode,
			    (proxy_mode == PROXY_MODE_GLOBAL), (gpointer)1);
	add_menu_separator(menu);
	add_menu_item(menu, "Edit servers", mevt_servers);
	tmp = add_menu_item(menu, "Set default server...", NULL);
	defserver_menu = tmp;
	update_defserver_menu_list();
	add_menu_item(menu, "Update GFWList", NULL);
	add_menu_separator(menu);
	add_menu_item(menu, "About", mevt_quit);
	add_menu_item(menu, "Quit", mevt_quit);

	app_indicator_set_menu(indicator, GTK_MENU(menu));

	gtk_widget_show_all(menu);

	gtk_main();

	conf_set_int(kfile, CONF_PROXY_MODE, proxy_mode);
	conf_close(kfile, conff);
	return 0;
}
