#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "common.h"

void html_make(char *filename);
void markdown_make(char *filename);
void asciidoc_make(char *filename);
void prepare_html_view();
void update_html_view();

/* Loading the gui - see ui.glade and the generated ui.c */
extern char ui_glade[];
extern int ui_glade_len;

/* GUI components */
GtkBuilder *builder;
GtkAboutDialog *ad;
GtkTextView *textview;
GtkViewport *view_viewport;
GtkContainer *view_container;
GtkScrolledWindow *view_scroll;
GtkLayout *view_layout;

/* Update control */
GThread *update_thread;
GCond *updated_cond;
GMutex *updated_mutex;

char *source_filename = NULL;
char *result_content = NULL;
enum result_type_e result_type;

enum source_type_e source_type_from_ext(char *);
void do_update_view();
gboolean do_update_view_(gpointer);
gpointer updater (gpointer);

static gboolean key_press_event(GtkWidget *widget, GdkEvent *event, GtkLabel *label) {
	GdkEventKey k = event->key;
	if (k.state & GDK_CONTROL_MASK) {
		switch (k.keyval) {
		case 'h':
			gtk_widget_show(GTK_WIDGET(ad));
			return TRUE;
		case 'r':
			do_update_view();
			return TRUE;
		}
	}

	return FALSE; // fall through
}

static gboolean modification_made(GtkWidget *widget, GdkEvent *event, GtkLabel *label) {
	puts ("Anything?");
	g_cond_signal(updated_cond);
	// all parameters unused
	return FALSE;
}

int main (int argc, char *argv[]) {
	GtkWidget *window;

	gtk_init(&argc, &argv);
	g_thread_init(NULL);

	updated_cond = g_cond_new();
	updated_mutex = g_mutex_new();

	builder = gtk_builder_new ();
	gtk_builder_add_from_string(builder, ui_glade, ui_glade_len, NULL);

	window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	ad = GTK_ABOUT_DIALOG (gtk_builder_get_object(builder, "about"));
	textview = GTK_TEXT_VIEW (gtk_builder_get_object(builder, "textview"));
	view_container = GTK_CONTAINER (gtk_builder_get_object(builder, "view_container"));
	//gtk_widget_set_size_request(GTK_WIDGET(view_viewport), 500, -1);

	gtk_builder_connect_signals (builder, NULL);
	g_object_unref (G_OBJECT (builder));
	
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(window, "key-press-event", G_CALLBACK (key_press_event), NULL);
	g_signal_connect(gtk_text_view_get_buffer(textview), "changed", G_CALLBACK(modification_made), NULL);
	/*g_signal_connect(textview, "insert-at-cursor", G_CALLBACK(modification_made), NULL);
	g_signal_connect(textview, "delete-from-cursor", G_CALLBACK(modification_made), NULL);
	g_signal_connect(textview, "backspace", G_CALLBACK(modification_made), NULL);
	g_signal_connect(textview, "paste-clipboard", G_CALLBACK(modification_made), NULL);*/

	// TODO actual option parsing
	if (argc == 2) {
		source_filename = argv[1];
		// read the file, if it exists
		gchar *source;
		g_file_get_contents(source_filename, &source, NULL, NULL);

		GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
		gtk_text_buffer_set_text(buffer, source, -1);
	} else {
		close(g_file_open_tmp("mupit.scratchXXXXXX", &source_filename, NULL));
		gchar *source = "I <em>should</em> put a welcome message here...";
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
		gtk_text_buffer_set_text(buffer, source, -1);
	}


	do_update_view();
	update_thread = g_thread_create(updater, NULL, TRUE, NULL);

	gtk_container_add(GTK_CONTAINER(view_container), view_widget);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

enum source_type_e source_type_from_ext(char *ext) {
	if (g_strrstr(ext, ".htm")) return HTML_SRC;
	if (g_strrstr(ext, ".md")) return MARKDOWN_SRC;
	if (g_strrstr(ext, ".txt")) return ASCIIDOC_SRC;
	if (g_strrstr(ext, ".tex")) return TEX_SRC;
	return HTML_SRC; // TODO this should probably be an error
}

void do_save() {
	GtkTextIter start;
	GtkTextIter end;

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);

	/* Obtain iters for the start and end of points of the buffer */
	gtk_text_buffer_get_start_iter (buffer, &start);
	gtk_text_buffer_get_end_iter (buffer, &end);

	/* Get the entire buffer text. */
	gchar *text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
	g_file_set_contents(source_filename, text, -1, NULL);
}

gboolean do_update_view_(gpointer data) {
	// argument is ignored
	do_update_view();
	return TRUE;
}

gpointer updater(gpointer data) {
	// argument is ignored
	g_mutex_lock(updated_mutex);
	while (TRUE) {
		g_cond_wait(updated_cond, updated_mutex);
		do_update_view();
	}
	g_mutex_unlock(updated_mutex);
	return NULL;
}

void do_update_view() {
	// TODO don't do anything if nothing's changed
	do_save();
	enum source_type_e source_type = source_type_from_ext(source_filename);
	switch (source_type) {
	case HTML_SRC:
		html_make(source_filename);
		break;
	case MARKDOWN_SRC:
		markdown_make(source_filename);
		break;
	case ASCIIDOC_SRC:
		asciidoc_make(source_filename);
	case TEX_SRC:
		break;
	}

	switch (result_type) {
	case HTML:
		prepare_html_view();
		update_html_view();
		break;
	}
}

char *find_tool(char *t) {
	int len = strlen(t) + 20;
	gchar *buf = g_new(gchar, len);
	g_snprintf(buf, len, "/usr/bin/%s", t); // TODO actually search the path
	return buf;
}

