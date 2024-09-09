CFLAGS = -ansi -pedantic -Wall

all: shell

hy345sh.o: hy345sh.c
	gcc $(CFLAGS) -c hy345sh.c;

shell: hy345sh.o
	gcc $(CFLAGS) -o hy345sh hy345sh.o;
	./hy345sh;

clean:
	rm -rf *.o;
	rm -rf hy345sh;
	rm -rf a.out
