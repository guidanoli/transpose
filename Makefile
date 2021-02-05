INSTALL_LOC=/usr/local/bin
INSTALL=install -p -m 0755

all: transpose note2aoff

transpose: transpose.c
	${CC} $< -o $@

note2aoff: note2aoff.c
	${CC} $< -o $@

install: transpose note2aoff
	${INSTALL} $^ ${INSTALL_LOC}

.PHONY: install
