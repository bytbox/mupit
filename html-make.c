#include <glib.h>

#include "common.h"

void html_make(char *filename) {
	g_file_get_contents(filename, &result_content, NULL, NULL);
	result_type = HTML;
}

