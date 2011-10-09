#include <gtk/gtk.h>

#include <gtkhtml/gtkhtml.h>

#include "common.h"

void prepare_html_view() {
	view_widget = gtk_html_new();
}

