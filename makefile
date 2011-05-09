CC=gcc

all: adrastea

ping.o: 
	$(CC) ./modules/ping.c -c -fPIC -o ./modules/ping.o

ping.so: ping.o
	$(CC) ./modules/ping.o -rdynamic -shared -o ./lib/ping.so

identify.o: 
	$(CC) ./modules/identify.c -c -fPIC -o ./modules/identify.o

identify.so: identify.o
	$(CC) ./modules/identify.o -rdynamic -shared -o ./lib/identify.so

join.o: 
	$(CC) ./modules/join.c -c -fPIC -o ./modules/join.o

join.so: join.o
	$(CC) ./modules/join.o -rdynamic -shared -o ./lib/join.so

adrastea: ping.so identify.so join.so
	$(CC) src/adrastea.c -ldl -o adrastea

clean:
	rm adrastea lib/*.so
