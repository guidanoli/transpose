CC=cc

transpose: transpose.c
	${CC} $< -o $@
