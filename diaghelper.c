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

void diag_warning_show(GtkWidget *win, const gchar *msg)
{
	GtkWidget *msgdialog;

	msgdialog = gtk_message_dialog_new(win ? GTK_WINDOW(win) : NULL,
					   win ? GTK_DIALOG_MODAL : 0,
					   GTK_MESSAGE_WARNING,
					   GTK_BUTTONS_OK,
					   "%s", msg);
	gtk_dialog_run(GTK_DIALOG(msgdialog));
	gtk_widget_destroy(msgdialog);
}
