transpose: transpose.c
	${CC} $< -o $@

install: transpose
	install -p -m 0755 transpose /usr/local/bin

.PHONY: install
