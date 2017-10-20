CC=gcc


all: test_lecteurs_redacteurs version2

test_lecteurs_redacteurs: test_lecteurs_redacteurs.o
	$(CC) -o test_lecteurs_redacteurs test_lecteurs_redacteurs.o -lpthread


test_lecteurs_redacteurs.o: test_lecteurs_redacteurs.c
	$(CC) -c test_lecteurs_redacteurs.c 

version2: version2.o
	$(CC) -o version2 version2.o -lpthread


version2.o: version2.c
	$(CC) -c version2.c 

clean:
	rm -rf *.o

