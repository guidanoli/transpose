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
	char inbuff[BUFSIZ], outbuff[BUFSIZ*3];
	long int offset;
	int usebemol;
	int usescientific;
	size_t size;

	/* A minor scale notes offsets to A */
	static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

	/* A chromatic scale notes */
	static const char* const aoff2str[2][12] = {
		{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
		{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
	};

	if (argc < 2) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print accidents with b instead of #\n"
				"  -s  read scientific pitch notation\n", argv[0]);
		return 1;
	}

	offset = strtol(argv[1], &end, 10);

	if (end == argv[1] || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", argv[1]);
		return 1;
	}

	usebemol = has_option(argc, argv, "-b");
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
				strcpy(outbuffptr, aoff2str[usebemol][newaoff]);
				outbuffptr += strlen(outbuffptr);

				if (usescientific) {
					long int oct = strtol(inbuffptr + 1, &end, 10), newoct;
					if (end == inbuffptr + 1) {
						fprintf(stderr, "Note at '%s' not in scientific pitch notation\n", inbuffptr + 1);
						return 1;
					} else {
						int nchars;
						newoct = (oct * 12 + aoff - 3 + offset) / 12;
						if (newoct < 0) {
							fprintf(stderr, "Negative octave\n");
							return 1;
						}
						nchars = sprintf(outbuffptr, "%ld", newoct);
						if (nchars < 0) {
							fprintf(stderr, "Could not print long integer to output buffer\n");
							return 1;
						}
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
