#ifndef __MENUHELPER_H_
#define __MENUHELPER_H_

GtkWidget *add_menu_item(GtkWidget *menu, gchar *lable, gpointer callback);
GtkWidget *add_menu_check_item(GtkWidget *menu, gchar *lable, gpointer callback,
				      gboolean is_active);
GtkWidget *add_menu_radio_item(GtkWidget *menu, GSList **group, gchar *lable,
			       gpointer callback, gboolean is_active, gpointer udata);
void add_menu_separator(GtkWidget *menu);

#endif /* __MENUHELPER_H_ */
