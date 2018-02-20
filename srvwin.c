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

static GtkWidget *servers_window = NULL;
static GtkWidget *listbox;

gboolean on_servers_window_delete_event(GtkWidget *widget,
				        GdkEvent *event, gpointer data)
{
	servers_window = NULL;
	return FALSE;
	//gtk_widget_hide(widget);
	//return TRUE;
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
