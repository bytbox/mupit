#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "common.h"

char *tmppath(char *relp) {
	char *bd = (char *)tmp_dir;
	return g_build_path(G_DIR_SEPARATOR_S, bd, relp, NULL);
}

char *abspath(char *relp) {
	if (g_path_is_absolute(relp)) return relp;
	char *curdir = g_get_current_dir();
	return g_build_path(G_DIR_SEPARATOR_S, curdir, relp, NULL);
}

void replace_suffix(char *str, char *suff) {
	int len = strlen(str);
	// fall back from str[len] until we find '.'
	int i = len;
	while (str[i-1] != '.') {
		i--;
	}

	// now start writing suff from str[i]
	int j = 0;
	while (suff[j])
		str[i++] = suff[j++];
}

char *find_tool(char *t) {
	int len = strlen(t) + 20;
	gchar *buf = g_new(gchar, len);
	g_snprintf(buf, len, "/usr/bin/%s", t); // TODO actually search the path
	return buf;
}

