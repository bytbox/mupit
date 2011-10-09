#include <gtk/gtk.h>

#include <stdio.h>

void markdown_make(char *filename);

extern char ui_glade[];
extern int ui_glade_len;

GtkBuilder *builder;
GtkAboutDialog *ad;

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
	gtk_builder_connect_signals (builder, NULL);
	g_object_unref (G_OBJECT (builder));
	
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(window, "key-press-event", G_CALLBACK (key_press_event), NULL);

	gtk_widget_show(window);
	gtk_main();

	return 0;
}

