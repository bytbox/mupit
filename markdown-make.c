#include <glib.h>
#include <stdio.h>

#include "common.h"

extern char markdown_Markdown_pl[];
extern unsigned int markdown_Markdown_pl_len;

void markdown_make(char *filename) {
	char *perl_path = find_tool("perl");
	GError *err = NULL;

	int child_pid, standard_input, standard_output, standard_error;
	char *argv[] = {perl_path, "-", filename, NULL};
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
		&err
		);

	if (err) {
		fprintf(stderr, "SPAWN ERROR: %s\n", err->message);
		return;
	}

	// spit markdown into standard_input
	GIOChannel *c = g_io_channel_unix_new(standard_input);
	GIOChannel *r = g_io_channel_unix_new(standard_output);
	g_io_channel_write_chars(c, markdown_Markdown_pl, markdown_Markdown_pl_len, NULL, NULL);
	g_io_channel_flush(c, NULL);

	// and fetch the markdown from the other end
	// TODO pay attention to the error stream
	gchar *result;
	gsize result_len;
	g_io_channel_read_to_end(r, &result, &result_len, NULL);
	result_content = result;
	result_type = HTML;
}

