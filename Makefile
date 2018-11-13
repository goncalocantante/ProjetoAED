CFLAGS= -g -Wall -std=c99
VFLAGS= --leak-check=full

Proj:   tourist_func.o Proj.o 
	gcc $(CFLAGS) -o tuktuk tourist_func.o Proj.o

tourist_func.o: tourist_func.c tourist.h
	gcc $(CFLAGS) -c tourist_func.c

Proj.o: Proj.c tourist.h
	gcc $(CFLAGS) -c Proj.c

valgrind:
	valgrind $(VFLAGS) ./tuktuk file.cities

clean:
	rm -f *.o *.~ tuktuk *.gch

