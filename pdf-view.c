#include <gtk/gtk.h>

#include <evince-document.h>
#include <evince-view.h>

#include "common.h"

gboolean evince_inited = FALSE;
EvDocument *pdf_document = NULL;
EvDocumentModel *pdf_model = NULL;
EvView *pdf_view = NULL;

void init_pdf_view() {
	evince_inited = ev_init();
	pdf_view = EV_VIEW(ev_view_new());
}

void prepare_pdf_view() {
	if (!evince_inited)
		init_pdf_view();
	gdk_threads_enter();
	view_widget = GTK_WIDGET(pdf_view);
	gdk_threads_leave();
}

void update_pdf_view() {
	GError *err = NULL;
	gdk_threads_enter();
	pdf_document = ev_document_factory_get_document(result_content, &err);

	if (err) {
		fprintf(stderr, "EVINCE ERROR: %s\n", err->message);
		gdk_threads_leave();
		return;
	}

	pdf_model = ev_document_model_new_with_document(pdf_document);
	ev_view_set_model(pdf_view, pdf_model);
	gdk_threads_leave();
}

