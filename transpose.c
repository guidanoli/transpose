#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	INITIAL,
	LETTER,
} state_t;

typedef struct {
	long int offset;
	long int aoff;
	int usesharp;
} context_t;

static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

static const char * const aoff2str[2][12] = {
	{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
	{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
};

static state_t consume(int ch, state_t st, context_t *ctx)
{
	switch (st) {
	case INITIAL:
		if (ch >= 'A' && ch <= 'G') {
			ctx->aoff = ch2aoff[ch - 'A'];
			return LETTER;
		} else {
			if (ch != EOF) putchar(ch);
			return INITIAL;
		}
	case LETTER:
		if (ch == '#') {
			ctx->aoff = (ctx->aoff + 1) % 12;
			return LETTER;
		} else if (ch == 'b') {
			ctx->aoff = (ctx->aoff + 11) % 12;
			return LETTER;
		} else {
			printf("%s", aoff2str[ctx->usesharp][(ctx->aoff + ctx->offset + 12) % 12]);
			if (ch != EOF) putchar(ch);
			return INITIAL;
		}
	}
	return st;
}

static long int str2long(const char* nptr)
{
	char *end;
	long int l;
	l = strtol(nptr, &end, 10);
	if (end == nptr || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", nptr);
		exit(1);
	}
	return l;
}

static int hasoption(int argc, char **argv, const char *opt)
{
	while(argc--) {
		if (strcmp(*argv, opt) == 0)
			return 1;
		++argv;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int ch;
	state_t st = INITIAL;
	context_t ctx;

	if (argc < 2 || hasoption(argc, argv, "-h")) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print output with flat (b) instead sharp (#)\n"
				"  -h  print usage information and exit\n", argv[0]);
		return argc < 2;
	}

	ctx.offset = str2long(argv[1]) % 12;
	ctx.usesharp = hasoption(argc, argv, "-b");

	do {
		ch = getchar();
		st = consume(ch, st, &ctx);
	} while (ch != EOF);

	return 0;
}
