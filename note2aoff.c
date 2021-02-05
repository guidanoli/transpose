#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	char inbuff[BUFSIZ], outbuff[BUFSIZ*2];

	/* Converts A(0) to G(6) to semi-tone offset relative to A */
	static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

	while (fgets(inbuff, sizeof(inbuff), stdin) != NULL) {
		char* outbuffptr = outbuff;
		for (char* inbuffptr = inbuff, inbuffch = inbuffptr[0]; inbuffch != '\0'; inbuffch = *(++inbuffptr)) {
			if (inbuffch >= 'A' && inbuffch <= 'G') {
				char accident = inbuffptr[1];
				int aoff = ch2aoff[inbuffch - 'A'];

				if (accident == '#') {
					aoff = (aoff + 1) % 12;
					++inbuffptr;
				} else if (accident == 'b') {
					aoff = (aoff + 11) % 12;
					++inbuffptr;
				}

				outbuffptr += sprintf(outbuffptr, "%d", aoff);
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
