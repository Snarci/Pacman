out: controllo.o pacman.o
	gcc -o out controllo.o pacman.o -lncurses -pthread -lm
pacman.o: pacman.c pacman.h
	gcc -c pacman.c -lncurses -pthread -lm
controllo.o: controllo.c pacman.h
	gcc -c controllo.c -lncurses -pthread -lm

clean:
	rm -f *.o
	