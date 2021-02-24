#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	INITIAL,
	LETTER,
} state_t;

static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

static const char* const aoff2str[2][12] = {
	{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
	{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
};


static char letter;
static int usesharp;
static long int offset;

static int has_option(int argc, char** argv, const char* opt)
{
	while(argc--) {
		if (strcmp(*argv, opt) == 0)
			return 1;
		++argv;
	}
	return 0;
}

static state_t consume(char ch, state_t st)
{
	switch (st) {
	case INITIAL:
	{
		if (ch >= 'A' && ch <= 'G') {
			letter = ch;
			return LETTER;
		} else {
			if (ch != '\0')
				putchar(ch);
			return INITIAL;
		}
	}
	case LETTER:
	{
		long int aoff = ch2aoff[letter - 'A'];
		int printlater = 0;
		int isletter = 0;

		if (ch == 'b') {
			aoff -= 1;
		} else if (ch == '#') {
			aoff += 1;
		} else if (ch >= 'A' && ch <= 'G') {
			letter = ch;
			isletter = 1;
		} else if (ch != '\0') {
			printlater = 1;
		}

		aoff = (aoff + offset + 12) % 12;
		printf("%s", aoff2str[usesharp][aoff]);

		if (printlater)
			putchar(ch);
	
		if (isletter)
			return LETTER;
		else
			return INITIAL;
	}
	default:
		fprintf(stderr, "Invalid state\n");
		exit(1);
		return st;
	}
}

int main(int argc, char** argv)
{
	char* end;
	char inbuff[BUFSIZ];
	state_t state = INITIAL;

	if (argc < 2 || has_option(argc, argv, "-h")) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print output with flat (b) instead sharp (#)\n"
				"  -h  print usage information and exit\n"
				"  -l  print buffer limit and exit\n", argv[0]);
		return argc < 2;
	} else if (has_option(argc, argv, "-l")) {
		printf("%zu\n", (size_t) BUFSIZ);
		return 0;
	}

	offset = strtol(argv[1], &end, 10) % 12;

	if (end == argv[1] || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", argv[1]);
		return 1;
	}

	usesharp = has_option(argc, argv, "-b");

	while (fgets(inbuff, sizeof(inbuff), stdin) != NULL)
		for (char* inbuffptr = inbuff; *inbuffptr != '\0'; ++inbuffptr)
			state = consume(*inbuffptr, state);

	consume('\0', state);

	return 0;
}
