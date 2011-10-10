#include <gtk/gtk.h>

#include <gtkhtml/gtkhtml.h>

#include <string.h>

#include "common.h"

GtkHTML *gtk_html = NULL;

void init_html_view() {
	gdk_threads_enter();
	gtk_html = GTK_HTML(gtk_html_new());
	gdk_threads_leave();
}

void prepare_html_view() {
	if (!gtk_html)
		init_html_view();
	gdk_threads_enter();
	view_widget = GTK_WIDGET(gtk_html);
	gdk_threads_leave();
}

void update_html_view() {
	gdk_threads_enter();
	if (strlen(result_content) > 0)
		gtk_html_load_from_string(gtk_html, result_content, -1);
	gdk_threads_leave();
}

