#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc < 3) goto usage;

	char* pat = argv[1];
	char** subs = argv + 2;

	char* old = malloc(strlen(pat));
	if (old == NULL) err(1, "malloc");

	size_t size = strlen(pat);
	for (int i = 2; i < argc; i++) size += strlen(argv[i]);
	char* new = malloc(size);
	if (new == NULL) err(1, "malloc");

	/* reconstruct old file name, build new file name */

	#define append(s, c) \
		do { \
			s[strlen(s)] = c; \
			s[strlen(s)+1] = 0; \
		} while (0)

	int in_brackets = 0;
	int j = 0; // subs index

	for (int i = 0; i < (int)strlen(pat); i++) {
		switch (pat[i]) {
		case '\\':
			switch (pat[i+1]) {
			case '/':
				i++;
				append(old, pat[i]);
				append(new, pat[i]);
				break;
			case '\\':
				append(old, pat[i]);
				append(new, pat[i]);
				i++;
				break;
			}
			break;
		case '[':
		case ']':
			if (in_brackets) { in_brackets = 0; continue; }

			in_brackets = 1;
			if (j == argc - 2) goto usage; // subs too few 
			strcat(new, subs[j]);
			j++;
			break;
		default:
			append(old, pat[i]);
			if (!in_brackets) append(new, pat[i]);
		}
	}

	if (in_brackets) goto usage;

	int r = rename(old, new);
	if (r == -1) err(1, NULL);

	free(old);
	free(new);

	return 0;

	usage:
	fprintf(stderr, "usage: %s pattern substitutions ...\n", argv[0]);
	return 1;
}
