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

#include "srvwin.h"
#include "paths.h"
#include "conf.h"
#include "core.h"

void update_defserver_menu_list();    /* in main.c */

char *methods[] = {
	"rc4-md5",
	"aes-128-gcm",
	"aes-192-gcm",
	"aes-256-gcm",
	"aes-128-cfb",
	"aes-192-cfb",
	"aes-256-cfb",
	"aes-128-ctr",
	"aes-192-ctr",
	"aes-256-ctr",
	"camellia-128-cfb",
	"camellia-192-cfb",
	"camellia-256-cfb",
	"bf-cfb",
	"chacha20-ietf-poly1305",
	"salsa20",
	"chacha20",
	"chacha20-ietf"
};

static GtkWidget *servers_window = NULL;
static GtkWidget *listbox;

static GtkWidget *edit_window = NULL;
static GtkWidget *nick, *srvaddr, *srvport, *localport, *passwd, *method, *method_entry;

static void add_listbox_item(GtkWidget *lb, const gchar *text)
{
	GtkWidget *row = gtk_list_box_row_new();
	GtkWidget *item = gtk_label_new(text);

	gtk_label_set_justify(GTK_LABEL(item), GTK_JUSTIFY_LEFT);
	//gtk_widget_set_valign(item, GTK_ALIGN_START);
	gtk_widget_set_halign(item, GTK_ALIGN_START);

	gtk_widget_set_margin_top(item, 4);
	gtk_widget_set_margin_bottom(item, 4);
	gtk_widget_set_margin_start(item, 4);

	gtk_container_add(GTK_CONTAINER(row), item);

	gtk_list_box_insert(GTK_LIST_BOX(lb), row, -1);

	gtk_widget_show_all(row);
}

static void flush_list()
{
	DIR* dir;
	struct dirent *dent;

	if ((dir = opendir(srvdir)) == NULL)
		return;

	while ((dent = readdir(dir)) != NULL) {
		if (dent->d_name[0] != '.') {
			add_listbox_item(listbox, dent->d_name);
		}
	}

	closedir(dir);
}

static void set_editwin_text(GtkWidget *row)
{
	GtkWidget *label;
	const char *nickname;
	char path[2048];
	GKeyFile *kfile;

	label = gtk_bin_get_child(GTK_BIN(row));
	nickname = gtk_label_get_text(GTK_LABEL(label));

	gtk_entry_set_text(GTK_ENTRY(nick), nickname);

	snprintf(path, 2048, "%s/%s", srvdir, nickname);
	kfile = conf_open(path);

	gtk_entry_set_text(GTK_ENTRY(srvaddr), conf_get_string(kfile, SERVER_ADDR));
	gtk_entry_set_text(GTK_ENTRY(srvport), conf_get_string(kfile, SERVER_PORT));
	gtk_entry_set_text(GTK_ENTRY(localport), conf_get_string(kfile, SERVER_LOCAL_PORT));
	gtk_entry_set_text(GTK_ENTRY(passwd), conf_get_string(kfile, SERVER_PASSWD));
	gtk_entry_set_text(GTK_ENTRY(method_entry), conf_get_string(kfile, SERVER_METHOD));

	conf_close(kfile, NULL);
}

static void on_edit_btn_ok_clicked(GtkButton *button, gpointer user_data)
{
	const char *_nick = gtk_entry_get_text(GTK_ENTRY(nick));
	const char *_srvaddr = gtk_entry_get_text(GTK_ENTRY(srvaddr));
	const char *_srvport = gtk_entry_get_text(GTK_ENTRY(srvport));
	const char *_localport = gtk_entry_get_text(GTK_ENTRY(localport));
	const char *_passwd = gtk_entry_get_text(GTK_ENTRY(passwd));
	//const char *_method = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(method));
	const char *_method = gtk_entry_get_text(GTK_ENTRY(method_entry));

	/* when edit window is showed by 'Add' button
	 * Do not allow nickname exists
	 */
	if ((user_data == NULL) && (core_server_exists(_nick) == 0)) {
		GtkWidget *msgdialog;
		msgdialog = gtk_message_dialog_new(GTK_WINDOW(edit_window),
						   GTK_DIALOG_MODAL,
						   GTK_MESSAGE_WARNING,
						   GTK_BUTTONS_CLOSE,
						   "This nickname is exists.");
		gtk_dialog_run(GTK_DIALOG(msgdialog));
		gtk_widget_destroy(msgdialog);
		return;
	}

	if (core_server_add(_nick, _srvaddr, _srvport, _localport, _passwd, _method) == 0) {
		if (user_data == NULL) {
			add_listbox_item(listbox, _nick);
		}
		update_defserver_menu_list();
	}

	gtk_window_close(GTK_WINDOW(edit_window));
}

void on_edit_btn_cancel_clicked(GtkButton *button, gpointer user_data)
{
	gtk_window_close(GTK_WINDOW(edit_window));
}

static void create_edit_window(GtkWidget *row)
{
	GtkBuilder *builder;
	GtkWidget *okbutton;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, EDITWIN_GLADE_FILE_PATH, NULL);
	gtk_builder_connect_signals(builder, NULL);

	edit_window = GTK_WIDGET(gtk_builder_get_object(builder, "edit_window"));

	okbutton = GTK_WIDGET(gtk_builder_get_object(builder, "edit_btn_ok"));
	g_signal_connect(okbutton, "clicked", G_CALLBACK(on_edit_btn_ok_clicked), row);

	nick = GTK_WIDGET(gtk_builder_get_object(builder, "nickname"));
	srvaddr = GTK_WIDGET(gtk_builder_get_object(builder, "srvaddr"));
	srvport = GTK_WIDGET(gtk_builder_get_object(builder, "srvport"));
	localport = GTK_WIDGET(gtk_builder_get_object(builder, "localport"));
	passwd = GTK_WIDGET(gtk_builder_get_object(builder, "passwd"));
	method = GTK_WIDGET(gtk_builder_get_object(builder, "method"));
	method_entry = GTK_WIDGET(gtk_builder_get_object(builder, "method-entry"));

	for (int i = 0; i < sizeof(methods)/sizeof(methods[0]); i++) {
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(method), NULL, methods[i]);
	}

	/* when edit window is showed by 'Edit' button.
	 * Get nickname and read config to show
	 */
	if (row != NULL) {
		gtk_widget_set_sensitive(nick, FALSE);
		set_editwin_text(row);
	}

	gtk_widget_show(edit_window);
	gtk_window_set_transient_for(GTK_WINDOW(edit_window), GTK_WINDOW(servers_window));
	gtk_window_set_modal(GTK_WINDOW(edit_window), TRUE);

	g_object_unref(builder);
}

gboolean on_servers_window_delete_event(GtkWidget *widget,
				        GdkEvent *event, gpointer data)
{
	servers_window = NULL;
	return FALSE;
	//gtk_widget_hide(widget);
	//return TRUE;
}

void on_btn_add_clicked(GtkButton *button, gpointer user_data)
{
	create_edit_window(NULL);
}

void on_btn_del_clicked(GtkButton *button, gpointer user_data)
{
	GtkListBoxRow *row;
	GtkWidget *label;
	const gchar *server;

	if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox))) == NULL) {
		return;
	}

	label = gtk_bin_get_child(GTK_BIN(row));
	server = gtk_label_get_text(GTK_LABEL(label));

	if (core_server_del(server) == 0) {
		gtk_container_remove(GTK_CONTAINER(listbox), GTK_WIDGET(row));
		update_defserver_menu_list();
	}
}

void on_btn_edit_clicked(GtkButton *button, gpointer user_data)
{
	GtkListBoxRow *row;

	if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox))) == NULL) {
		return;
	}

	create_edit_window(GTK_WIDGET(row));
}

void srvwin_show()
{
	GtkBuilder *builder;

	if (!servers_window) {
		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder, SRVWIN_GLADE_FILE_PATH, NULL);
		gtk_builder_connect_signals(builder, NULL);

		servers_window = GTK_WIDGET(gtk_builder_get_object(builder, "servers_window"));
		listbox = GTK_WIDGET(gtk_builder_get_object(builder, "srvwin_listbox"));

		flush_list();

		g_object_unref(builder);
	}

	gtk_widget_show(servers_window);
	gtk_window_present(GTK_WINDOW(servers_window));
}
