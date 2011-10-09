#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: %s BINFILE CFILE\n", argv[0]);
		return 1;
	}

	unsigned int i;
	char *varname = malloc(strlen(argv[1])+1);
	for (i=0; i<=strlen(argv[1]); i++) {
		if (argv[1][i] == '.')
			varname[i] = '_';
		else if (argv[1][i] == '/')
			varname[i] = '_';
		else varname[i] = argv[1][i];
	}

	FILE *fin = fopen(argv[1], "rb");
	if (!fin) perror("could not open file for reading"), exit(1);
	FILE *fout = fopen(argv[2], "w");

	fprintf(fout, "char %s[] = {\n", varname);
	unsigned int size = 0;
	char c = fgetc(fin);
	while (!feof(fin)) {
		size++;
		fprintf(fout, "\t0x%.2x,\n", c);
		c = fgetc(fin);
	}
	fprintf(fout, "\t};\n");
	fprintf(fout, "unsigned int %s_len = %d;\n", varname, size);
	fclose(fin);
	fclose(fout);

	return 0;
}

