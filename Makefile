CFLAGS= -O3 -Wall -std=c99

Proj:   tourist_func.o Proj.o 
	gcc $(CFLAGS) -o tuktuk tourist_func.o Proj.o

tourist_func.o: tourist_func.c tourist.h
	gcc $(CFLAGS) -c tourist_func.c

Proj.o: Proj.c tourist.h
	gcc $(CFLAGS) -c Proj.c

clean:
	rm -f *.o *.~ tuktuk *.gch
