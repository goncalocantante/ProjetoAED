CFLAGS= -g -Wall -std=c99
VFLAGS= --leak-check=full

main:   tourist_func.o main.o modoA.o modoB.o
	gcc $(CFLAGS) -o tuktuk tourist_func.o main.o modoA.o modoB.o

tourist_func.o: tourist_func.c tourist.h
	gcc $(CFLAGS) -c tourist_func.c

main.o: main.c tourist.h
	gcc $(CFLAGS) -c main.c

modoA.o: modoA.c tourist.h modos.h
	gcc $(CFLAGS) -c modoA.c

modoB.o: modoB.c tourist.h modos.h
	gcc $(CFLAGS) -c modoB.c

valgrind:
	valgrind $(VFLAGS) ./tuktuk file.cities

clean:
	rm -f *.o *.~ tuktuk *.gch
