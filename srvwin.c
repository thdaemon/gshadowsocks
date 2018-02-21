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
#include "core.h"

static GtkWidget *servers_window = NULL;
static GtkWidget *listbox;

static GtkWidget *edit_window = NULL;
static GtkWidget *nick, *srvaddr, *srvport, *localport, *passwd, *method;

static void create_edit_window(const char *srvname)
{
	GtkBuilder *builder;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, EDITWIN_GLADE_FILE_PATH, NULL);
	gtk_builder_connect_signals(builder, NULL);

	edit_window = GTK_WIDGET(gtk_builder_get_object(builder, "edit_window"));

	nick = GTK_WIDGET(gtk_builder_get_object(builder, "nickname"));
	srvaddr = GTK_WIDGET(gtk_builder_get_object(builder, "srvaddr"));
	srvport = GTK_WIDGET(gtk_builder_get_object(builder, "srvport"));
	localport = GTK_WIDGET(gtk_builder_get_object(builder, "localport"));
	passwd = GTK_WIDGET(gtk_builder_get_object(builder, "passwd"));
	method = GTK_WIDGET(gtk_builder_get_object(builder, "method"));

	gtk_widget_show_all(edit_window);
	gtk_window_set_transient_for(GTK_WINDOW(edit_window), GTK_WINDOW(servers_window));
	gtk_window_set_modal(GTK_WINDOW(edit_window), TRUE);

	g_object_unref(builder);
}

static void add_listbox_item(GtkWidget *lb, gchar *text)
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
	}
}

void on_btn_edit_clicked(GtkButton *button, gpointer user_data)
{
	GtkListBoxRow *row;
	GtkWidget *label;

	if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox))) == NULL) {
		return;
	}

	label = gtk_bin_get_child(GTK_BIN(row));
	create_edit_window(gtk_label_get_text(GTK_LABEL(label)));
}

void on_edit_btn_ok_clicked(GtkButton *button, gpointer user_data)
{
	const char *_nick = gtk_entry_get_text(GTK_ENTRY(nick));
	const char *_srvaddr = gtk_entry_get_text(GTK_ENTRY(srvaddr));
	const char *_srvport = gtk_entry_get_text(GTK_ENTRY(srvport));
	const char *_localport = gtk_entry_get_text(GTK_ENTRY(localport));
	const char *_passwd = gtk_entry_get_text(GTK_ENTRY(passwd));
	const char *_method = "";
	core_server_add(_nick, _srvaddr, _srvport, _localport, _passwd, _method);

	gtk_window_close(GTK_WINDOW(edit_window));
}

void on_edit_btn_cancel_clicked(GtkButton *button, gpointer user_data)
{
	gtk_window_close(GTK_WINDOW(edit_window));
}

int srvwin_oncreate(GtkWidget *window)
{
	DIR* dir;
	struct dirent *dent;

	if ((dir = opendir(srvdir)) == NULL)
		return -1;

	while ((dent = readdir(dir)) != NULL) {
		if (dent->d_name[0] != '.') {
			add_listbox_item(listbox, dent->d_name);
		}
	}

	closedir(dir);
	return 0;
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

		srvwin_oncreate(servers_window);

		g_object_unref(builder);
	}

	gtk_widget_show(servers_window);
	gtk_window_present(GTK_WINDOW(servers_window));
}
