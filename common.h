#ifndef COMMON_H
#define COMMON_H

#include <gtk/gtk.h>

enum source_type_e { HTML_SRC, MARKDOWN_SRC, ASCIIDOC_SRC, TEX_SRC };

extern char *result_content;
extern enum result_type_e { HTML, PDF, DVI } result_type;

GtkWidget *view_widget;

extern const char *tmp_dir;

char *find_tool(char *);
char *abspath(char *);
char *tmppath(char *);
void replace_suffix(char *, char *);

#endif /* !COMMON_H */

