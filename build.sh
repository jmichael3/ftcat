if [ ! -d bin ]
then
	mkdir bin
fi
gcc -std=c11 -pedantic-errors -Wall -Wextra -o bin/ftcat src/ftcat.c && strip bin/ftcat
