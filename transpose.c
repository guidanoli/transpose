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


static int letter, usesharp;
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

static state_t consume(int ch, state_t st)
{
	switch (st) {
	case INITIAL:
	{
		if (ch >= 'A' && ch <= 'G') {
			letter = ch;
			st = LETTER;
		} else if (ch != EOF) {
			putchar(ch);
		}
		break;
	}
	case LETTER:
	{
		long int aoff = ch2aoff[letter - 'A'];
		int misc = 0;

		if (ch == 'b') {
			aoff -= 1;
			st = INITIAL;
		} else if (ch == '#') {
			aoff += 1;
			st = INITIAL;
		} else if (ch >= 'A' && ch <= 'G') {
			letter = ch;
		} else if (ch != EOF) {
			misc = 1;
			st = INITIAL;
		}

		aoff = (aoff + offset + 12) % 12;
		printf("%s", aoff2str[usesharp][aoff]);

		if (misc) putchar(ch);

		break;
	}
	default:
		fprintf(stderr, "Invalid state\n");
		exit(1);
	}
	return st;
}

int main(int argc, char** argv)
{
	char* end;
	int ch;
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

	do {
		ch = getchar();
		state = consume(ch, state);
	} while (ch != EOF);

	return 0;
}
