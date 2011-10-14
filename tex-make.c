#include <glib.h>
#include <string.h>

#include "common.h"

#define TEX_PROGRAM ("pdflatex")

void tex_make_pdf(char *filename) {
	char *tex_path = find_tool(TEX_PROGRAM);
	GError *err = NULL;

	int child_pid, standard_input, standard_output, standard_error;
	char *argv[] = {
		tex_path,
		"-interaction=batchmode",
		"-output-directory",
		(gchar *)tmp_dir,
		abspath(filename),
		NULL};
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

	GIOChannel *in = g_io_channel_unix_new(standard_input);
	GIOChannel *out = g_io_channel_unix_new(standard_output);
	GIOChannel *errput = g_io_channel_unix_new(standard_error);
	g_io_channel_shutdown(in, TRUE, NULL); // nothing to put to standard input

	// read all of stdin and stderr
	gchar *output, *errors;
	gsize output_len, errors_len;
	g_io_channel_read_to_end(out, &errors, &errors_len, NULL);
	g_io_channel_read_to_end(errput, &output, &output_len, NULL);

	g_io_channel_shutdown(out, TRUE, NULL);
	g_io_channel_shutdown(errput, TRUE, NULL);
	g_io_channel_unref(in);
	g_io_channel_unref(out);
	g_io_channel_unref(errput);
	result_content = g_filename_to_uri(tmppath(g_strndup(filename, strlen(filename)+5)), NULL, &err);
	if (err) {
		fprintf(stderr, "FILENAME_TO_URI ERROR: %s\n", err->message);
		return;
	}
	replace_suffix(result_content, "pdf");
	result_type = PDF;
}

void tex_make(char *filename) {
	tex_make_pdf(filename);
}

