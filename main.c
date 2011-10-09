#include <gtk/gtk.h>

#include <stdio.h>

#include "common.h"

void markdown_make(char *filename);
void prepare_html_view();
void update_html_view();

extern char ui_glade[];
extern int ui_glade_len;

GtkBuilder *builder;
GtkAboutDialog *ad;
GtkTextView *textview;
GtkViewport *view_viewport;

char *result_content;
enum result_type_e result_type;

static gboolean key_press_event(GtkWidget *widget, GdkEvent *event, GtkLabel *label) {
	GdkEventKey k = event->key;
	if (k.state & GDK_CONTROL_MASK) {
		if (k.keyval == 'h') {
			gtk_widget_show(GTK_WIDGET(ad));
		}
		return TRUE;
	}
	return FALSE; // fall through
}

int main (int argc, char *argv[]) {
	GtkWidget *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_string(builder, ui_glade, ui_glade_len, NULL);

	window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	ad = GTK_ABOUT_DIALOG (gtk_builder_get_object(builder, "about"));
	textview = GTK_TEXT_VIEW (gtk_builder_get_object(builder, "textview"));
	view_viewport = GTK_VIEWPORT (gtk_builder_get_object(builder, "view_viewport"));

	gtk_builder_connect_signals (builder, NULL);
	g_object_unref (G_OBJECT (builder));
	
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(window, "key-press-event", G_CALLBACK (key_press_event), NULL);

	markdown_make("LICENSE");
	prepare_html_view();
	update_html_view();
	gtk_container_add(GTK_CONTAINER(view_viewport), view_widget);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

