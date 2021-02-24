#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int has_option(int argc, char** argv, const char* opt)
{
	while(argc--) {
		if (strcmp(*argv, opt) == 0)
			return 1;
		++argv;
	}
	return 0;
}

int main(int argc, char** argv)
{
	char* end;
	char inbuff[BUFSIZ], outbuff[BUFSIZ*2];
	long int offset;
	int usesharp;
	int usescientific;

	/* A minor scale notes offsets to A */
	static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

	/* A chromatic scale notes */
	static const char* const aoff2str[2][12] = {
		{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
		{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
	};

	if (argc < 2 || has_option(argc, argv, "-h")) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print output with flat (b) instead sharp (#)\n"
				"  -s  read input in ASPN\n"
				"  -h  print usage information and exit\n"
				"  -l  print buffer limit and exit\n", argv[0]);
		return argc < 2;
	} else if (has_option(argc, argv, "-l")) {
		printf("%zu\n", (size_t) BUFSIZ);
		return 0;
	}

	offset = strtol(argv[1], &end, 10);

	if (end == argv[1] || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", argv[1]);
		return 1;
	}

	usesharp = has_option(argc, argv, "-b");
	usescientific = has_option(argc, argv, "-s");

	while (fgets(inbuff, sizeof(inbuff), stdin) != NULL) {
		char* outbuffptr = outbuff;
		for (char* inbuffptr = inbuff, inbuffch = inbuffptr[0]; inbuffch != '\0'; inbuffch = *(++inbuffptr)) {
			if (inbuffch >= 'A' && inbuffch <= 'G') {
				char accident = inbuffptr[1];
				long int aoff = ch2aoff[inbuffch - 'A'], newaoff;

				if (accident == '#') {
					aoff = aoff + 1;
					++inbuffptr;
				} else if (accident == 'b') {
					aoff = aoff - 1;
					++inbuffptr;
				}

				newaoff = (((aoff + offset) % 12) + 12) % 12;
				strcpy(outbuffptr, aoff2str[usesharp][newaoff]);
				outbuffptr += strlen(outbuffptr);

				if (usescientific) {
					long int oct = strtol(inbuffptr + 1, &end, 10), newoct;
					if (end == inbuffptr + 1) {
						fprintf(stderr, "Note at '%s' not in scientific pitch notation\n", inbuffptr + 1);
						return 1;
					} else {
						int nchars;
						newoct = (oct * 12 + (aoff + 9) % 12 + offset) / 12;
						if (newoct < 0) {
							fprintf(stderr, "Negative octave\n");
							return 1;
						} else if (newoct > 9) {
							fprintf(stderr, "Too high octave\n");
							return 1;
						}
						nchars = sprintf(outbuffptr, "%ld", newoct);
						if (nchars < 0) {
							fprintf(stderr, "Could not print long integer to output buffer\n");
							return 1;
						}
						outbuffptr += nchars;
						inbuffptr = end - 1;
					}
					
				}
			} else {
				*(outbuffptr++) = inbuffch;
			}
		}
		*outbuffptr = '\0';
		if (fputs(outbuff, stdout) < 0) {
			fprintf(stderr, "Could not write '%s' to stdout\n", outbuff);
			return 1;
		}
	}

	return 0;
}
