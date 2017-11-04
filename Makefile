CC=gcc -Wall -Werror

all: redacteur_prio lecteur_prio file_prio file_prio2 file_prio3

test_red: redacteur_prio
	./redacteur_prio 5 5 5

test_lect: lecteur_prio
	./lecteur_prio 5 5 5

test_file_1: file_prio
	./file_prio 5 5 5

test_file_2: file_prio2
	./file_prio2 5 5 5

test_file_3: file_prio3
	./file_prio3 5 5 5

redacteur_prio: redacteur_prio.o
	$(CC) -o redacteur_prio redacteur_prio.o -lpthread


redacteur_prio.o: redacteur_prio.c
	$(CC) -c redacteur_prio.c

lecteur_prio: lecteur_prio.o
	$(CC) -o lecteur_prio lecteur_prio.o -lpthread


lecteur_prio.o: lecteur_prio.c
	$(CC) -c lecteur_prio.c

file_prio: file_prio.o thread_safe_list.o
	$(CC) -o file_prio file_prio.o thread_safe_list.o -lpthread


file_prio.o: file_prio.c
	$(CC) -c file_prio.c

thread_safe_list.o: thread_safe_list.c
	$(CC) -c thread_safe_list.c

file_prio2: file_prio2.o thread_safe_list_version2.o
	$(CC) -o file_prio2 file_prio2.o thread_safe_list_version2.o -lpthread


file_prio2.o: file_prio2.c
	$(CC) -c file_prio2.c

thread_safe_list_version2.o: thread_safe_list_version2.c
	$(CC) -c thread_safe_list_version2.c

file_prio3: file_prio3.o thread_safe_list_version3.o
	$(CC) -o file_prio3 file_prio3.o thread_safe_list_version3.o -lpthread


file_prio3.o: file_prio3.c
	$(CC) -c file_prio3.c

thread_safe_list_version3.o: thread_safe_list_version3.c
	$(CC) -c thread_safe_list_version3.c

clean:
	rm -rf *.o redacteur_prio lecteur_prio file_prio file_prio2 file_prio3

