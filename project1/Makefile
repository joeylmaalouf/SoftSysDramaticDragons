alias.o: alias.c shell.h
	gcc -Wall -c alias.c -o alias.o

memory.o: memory.c shell.h
	gcc -Wall -c memory.c -o memory.o

parsing.o: parsing.c shell.h
	gcc -Wall -c parsing.c -o parsing.o

shell: shell.c alias.o memory.o parsing.o shell.h
	gcc -Wall shell.c alias.o memory.o parsing.o -o shell
