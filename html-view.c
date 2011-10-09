#include <gtk/gtk.h>

#include <gtkhtml/gtkhtml.h>

#include <string.h>

#include "common.h"

GtkHTML *gtk_html = NULL;

void init_html_view() {
	gtk_html = GTK_HTML(gtk_html_new());
}

void prepare_html_view() {
	if (!gtk_html)
		init_html_view();
	view_widget = GTK_WIDGET(gtk_html);
}

void update_html_view() {
	gtk_html_load_from_string(gtk_html, result_content, strlen(result_content));
}

