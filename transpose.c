#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	INITIAL,
	LETTER,
	ACCIDENT,
} state_t;

typedef struct {
	long int offset;
	long int aoff;
	int usesharp;
} context_t;

static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

static const char* const aoff2str[2][12] = {
	{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
	{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
};

static int has_option(int argc, char** argv, const char* opt)
{
	while(argc--) {
		if (strcmp(*argv, opt) == 0)
			return 1;
		++argv;
	}
	return 0;
}

static state_t transpose(int ch, context_t* ctx)
{
	long int newaoff = (ctx->aoff + ctx->offset + 12) % 12;
	printf("%s", aoff2str[ctx->usesharp][newaoff]);
	if (ch != EOF) putchar(ch);
	return INITIAL;
}

static state_t consume(int ch, state_t st, context_t* ctx)
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
			ctx->aoff += 1;
			return ACCIDENT;
		} else if (ch == 'b') {
			ctx->aoff -= 1;
			return ACCIDENT;
		} else {
			return transpose(ch, ctx);
		}
	case ACCIDENT:
		return transpose(ch, ctx);
	}
	return st;
}

int main(int argc, char** argv)
{
	char* end;
	int ch;
	state_t st = INITIAL;
	context_t ctx;

	if (argc < 2 || has_option(argc, argv, "-h")) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print output with flat (b) instead sharp (#)\n"
				"  -h  print usage information and exit\n", argv[0]);
		return argc < 2;
	}

	ctx.offset = strtol(argv[1], &end, 10) % 12;

	if (end == argv[1] || *end != '\0') {
		fprintf(stderr, "Could not convert '%s' into a long int\n", argv[1]);
		return 1;
	}

	ctx.usesharp = has_option(argc, argv, "-b");

	do {
		ch = getchar();
		st = consume(ch, st, &ctx);
	} while (ch != EOF);

	return 0;
}
