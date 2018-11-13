CFLAGS= -g -Wall -std=c99
VFLAGS= --leak-check=full

main:   tourist_func.o main.o 
	gcc $(CFLAGS) -o tuktuk tourist_func.o main.o

tourist_func.o: tourist_func.c tourist.h
	gcc $(CFLAGS) -c tourist_func.c

main.o: main.c tourist.h
	gcc $(CFLAGS) -c main.c

valgrind:
	valgrind $(VFLAGS) ./tuktuk file.cities

clean:
	rm -f *.o *.~ tuktuk *.gch

