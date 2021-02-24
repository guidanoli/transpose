INSTALL_LOC=/usr/local/bin
INSTALL=install -p -m 0755

all: transpose 

transpose: transpose.c
	${CC} $< -o $@ ${CFLAGS}

install: transpose
	${INSTALL} $^ ${INSTALL_LOC}

.PHONY: install
