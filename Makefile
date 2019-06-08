CFLAGS = -O3 -Wall -Wno-missing-braces -Wextra -Wpedantic
CFLAGS += -Wno-unused-parameter -Wno-unused-variable

all: rn

install: rn rn.1
	cp -p rn /usr/local/bin
	cp -p rn.1 /usr/local/man/man1

uninstall: rn rn.1
	rm /usr/local/bin/rn
	rm /usr/local/man/man1/rn.1
