#include <glib.h>

#include "common.h"

void asciidoc_make(char *filename) {
	char *asciidoc_path = find_tool("asciidoc");

	int child_pid, standard_input, standard_output, standard_error;
	char *argv[] = {asciidoc_path, "-o", "-", filename, NULL};
	g_spawn_async_with_pipes(
		".",
		argv,
		NULL,
		0,
		NULL,
		NULL,
		&child_pid,
		&standard_input,
		&standard_output,
		&standard_error,
		NULL
		);
	// spit markdown into standard_input
	//GIOChannel *c = g_io_channel_unix_new(standard_input);
	GIOChannel *r = g_io_channel_unix_new(standard_output);
	//g_io_channel_write_chars(c, markdown_Markdown_pl, markdown_Markdown_pl_len, NULL, NULL);
	//g_io_channel_flush(c, NULL);

	// and fetch the markdown from the other end
	// TODO pay attention to the error stream
	gchar *result;
	gsize result_len;
	g_io_channel_read_to_end(r, &result, &result_len, NULL);
	result_content = result;
	result_type = HTML;
}

