all: adrastea

adrastea: src/adrastea.c
	cc src/adrastea.c -ldl -o adrastea

clean:
	rm adrastea lib/*.so
