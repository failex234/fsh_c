fedit: src/fsh.h src/log.c src/io.c src/builtins.c src/runner.c src/utils.c src/fsh.c
		$(CC) -g src/log.c src/io.c src/builtins.c src/runner.c src/utils.c src/fsh.c -o fsh -Wall -Wextra -pedantic

install: fsh
		cp fsh ${DESTDIR}/usr/bin

uninstall:
		rm -f ${DESTDIR}/usr/bin/fsh
