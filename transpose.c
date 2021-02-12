#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	char* end;
	char inbuff[BUFSIZ], outbuff[BUFSIZ*3];
	long int offset;
	size_t size;

	/* A minor scale notes offsets to A */
	static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

	/* A chromatic scale notes */
	static const char* const aoff2str[12] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };

	if (argc < 2) {
		fprintf(stderr, "Usage: %s OFFSET\n"
				"Transpose musical notes by offset\n", argv[0]);
		return 1;
	}

	offset = strtol(argv[1], &end, 10) % 12;

	if (end == argv[1] || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", argv[1]);
		return 1;
	}

	while (fgets(inbuff, sizeof(inbuff), stdin) != NULL) {
		char* outbuffptr = outbuff;
		for (char* inbuffptr = inbuff, inbuffch = inbuffptr[0]; inbuffch != '\0'; inbuffch = *(++inbuffptr)) {
			if (inbuffch >= 'A' && inbuffch <= 'G') {
				char accident = inbuffptr[1];
				int notevalue = ch2aoff[inbuffch - 'A'];

				if (accident == '#') {
					notevalue = (notevalue + 1) % 12;
					++inbuffptr;
				} else if (accident == 'b') {
					notevalue = (notevalue + 11) % 12;
					++inbuffptr;
				}

				notevalue = (notevalue + offset + 12) % 12;
				strcpy(outbuffptr, aoff2str[notevalue]);
				outbuffptr += strlen(outbuffptr);
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
