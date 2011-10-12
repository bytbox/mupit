#include <glib.h>

#include "common.h"

void asciidoc_make(char *filename) {
	char *asciidoc_path = find_tool("asciidoc");

	int child_pid, standard_output;
	char *argv[] = {asciidoc_path, "-o", "-", filename, NULL};
	g_spawn_async_with_pipes(
		".",
		argv,
		NULL,
		0,
		NULL,
		NULL,
		&child_pid,
		NULL,
		&standard_output,
		NULL,
		NULL
		);
	GIOChannel *r = g_io_channel_unix_new(standard_output);

	// TODO pay attention to the error stream
	gchar *result;
	gsize result_len;
	g_io_channel_read_to_end(r, &result, &result_len, NULL);
	g_io_channel_shutdown(r, TRUE, NULL);
	g_io_channel_unref(r);
	result_content = result;
	result_type = HTML;
}

