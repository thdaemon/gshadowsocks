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

GtkWidget *add_menu_item(GtkWidget *menu, gchar *lable, gpointer callback)
{
	GtkWidget *item;

	item = gtk_menu_item_new_with_label(lable);

	if (callback)
		g_signal_connect(item, "activate", G_CALLBACK(callback), item);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	return item;
}

GtkWidget *add_menu_check_item(GtkWidget *menu, gchar *lable, gpointer callback,
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

GtkWidget *add_menu_radio_item(GtkWidget *menu, GSList **group, gchar *lable,
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

void add_menu_separator(GtkWidget *menu)
{
	GtkWidget *item;
	item = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}
