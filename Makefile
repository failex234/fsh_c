fsh: src/fsh.h src/log.c src/io.c src/builtins.c src/runner.c src/utils.c src/terminal.c src/fsh.c
		$(CC) src/log.c src/io.c src/builtins.c src/runner.c src/utils.c src/terminal.c src/fsh.c src/configs.c -o fsh -Wall -Wextra -pedantic

install: fsh
		cp fsh ${DESTDIR}/usr/bin

uninstall:
		rm -f ${DESTDIR}/usr/bin/fsh
