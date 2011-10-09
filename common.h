#ifndef COMMON_H
#define COMMON_H

#include <gtk/gtk.h>

extern char *result_content;
extern enum result_type_e { HTML } result_type;
extern enum source_type_e { HTML_SRC, MARKDOWN_SRC } source_type;

GtkWidget *view_widget;

#endif /* !COMMON_H */

