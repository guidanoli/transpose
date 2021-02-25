#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	INITIAL,
	LETTER,
	ACCIDENT,
} state_t;

static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

static const char* const aoff2str[2][12] = {
	{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
	{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
};


static int usesharp;
static long int aoff, offset;

static int has_option(int argc, char** argv, const char* opt)
{
	while(argc--) {
		if (strcmp(*argv, opt) == 0)
			return 1;
		++argv;
	}
	return 0;
}

static state_t transpose(int ch)
{
	long int newaoff = (aoff + offset + 12) % 12;
	printf("%s", aoff2str[usesharp][newaoff]);
	if (ch != EOF) putchar(ch);
	return INITIAL;
}

static state_t consume(int ch, state_t st)
{
	switch (st) {
	case INITIAL:
		if (ch >= 'A' && ch <= 'G') {
			aoff = ch2aoff[ch - 'A'];
			return LETTER;
		} else {
			if (ch != EOF) putchar(ch);
			return INITIAL;
		}
	case LETTER:
		if (ch == '#') {
			aoff += 1;
			return ACCIDENT;
		} else if (ch == 'b') {
			aoff -= 1;
			return ACCIDENT;
		} else {
			return transpose(ch);
		}
	case ACCIDENT:
		return transpose(ch);
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
				"  -h  print usage information and exit\n", argv[0]);
		return argc < 2;
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
