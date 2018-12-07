CFLAGS= -g -Wall -std=c99 
VFLAGS= --leak-check=full

main: clean main.o problemas.o utils.o file.o queue.o
	gcc $(CFLAGS) -o tuktuk main.o problemas.o utils.o file.o queue.o

main.o: main.c file.h problemas.h 
	gcc $(CFLAGS) -c main.c

file.o: file.c file.h utils.h
	gcc $(CFLAGS) -c file.c


problemas.o: problemas.c problemas.h utils.h queue.h
	gcc $(CFLAGS) -c problemas.c


utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

queue.o: queue.c queue.h utils.h
	gcc $(CFLAGS) -c queue.c
	

valgrind:
	valgrind $(VFLAGS) ./tuktuk file.cities


clean:
	rm -f *.o *.~ tuktuk *.gch

cfiles:
	rm *.cities *.walks 
