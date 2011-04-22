all: src/adrastea.c
	gcc src/adrastea.c -ldl -o adrastea
