#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
	INITIAL,
	NOTE,
	OCTAVE,
} state_t;

typedef struct {
	long int offset;
	long int aoff;
	long int octave;
	int usesharp;
	int usespn;
} context_t;

static const char *program;

static const int ch2aoff[7] = { 0, 2, 3, 5, 7, 8, 10 };

static const char * const aoff2str[2][12] = {
	{ "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
	{ "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" }
};

static void error(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "%s: ", program);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	va_end(va);
	exit(1);
}

static state_t consume(int ch, state_t st, context_t *ctx)
{
	switch (st) {
	case INITIAL:
		if (ch >= 'A' && ch <= 'G') {
			ctx->aoff = ch2aoff[ch - 'A'];
			return NOTE;
		} else {
			if (ch != EOF) putchar(ch);
			return INITIAL;
		}
	case NOTE:
		if (ch == '#') {
			ctx->aoff = (ctx->aoff + 1) % 12;
			return NOTE;
		} else if (ch == 'b') {
			ctx->aoff = (ctx->aoff + 11) % 12;
			return NOTE;
		} else if (ctx->usespn && ch >= '0' && ch <= '9') {
			ctx->octave = ch - '0';
			return OCTAVE;
		} else {
			char newaoff = (ctx->aoff + ctx->offset % 12 + 12) % 12;
			printf("%s", aoff2str[ctx->usesharp][newaoff]);
			if (ch != EOF) putchar(ch);
			return INITIAL;
		}
	case OCTAVE:
		if (ch >= '0' && ch <= '9') {
			ctx->octave = ctx->octave * 10 + ch - '0';
			return OCTAVE;
		} else {
			char newaoff = (ctx->aoff + ctx->offset % 12 + 12) % 12;
			long int c0off = (ctx->aoff + 9) % 12 + ctx->octave * 12 + ctx->offset;
			if (c0off < 0) error("Could not tranpose to note lower than C0");
			printf("%s%ld", aoff2str[ctx->usesharp][newaoff], c0off / 12);
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

	if (end == nptr || *end != '\0')
		error("Could not convert '%s' into a long int", nptr);

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

	program = *argv;

	if (argc < 2 || hasoption(argc, argv, "-h")) {
		fprintf(stderr, "Transpose musical notes by offset\n"
				"Usage: %s OFFSET [OPTIONS]\n"
				"Options:\n"
				"  -b  print output with flat (b) instead sharp (#)\n"
				"  -h  print usage information and exit\n"
				"  -s  read input using scientific pitch notation\n", argv[0]);
		return argc < 2;
	}

	ctx.offset = str2long(argv[1]);
	ctx.usesharp = hasoption(argc, argv, "-b");
	ctx.usespn = hasoption(argc, argv, "-s");

	do {
		ch = getchar();
		st = consume(ch, st, &ctx);
	} while (ch != EOF);

	return 0;
}
