CC=gcc


all: version1 version2 version3 version4

version1: version1.o
	$(CC) -o version1 version1.o -lpthread


version1.o: version1.c
	$(CC) -c version1.c 

version2: version2.o
	$(CC) -o version2 version2.o -lpthread


version2.o: version2.c
	$(CC) -c version2.c 

version3: version3.o thread_safe_list.o
	$(CC) -o version3 version3.o thread_safe_list.o -lpthread


version3.o: version3.c
	$(CC) -c version3.c 

thread_safe_list.o: thread_safe_list.c
	$(CC) -c thread_safe_list.c

version4: version4.o thread_safe_list_version2.o
	$(CC) -o version4 version4.o thread_safe_list_version2.o -lpthread


version4.o: version4.c
	$(CC) -c version4.c 

thread_safe_list_version2.o: thread_safe_list_version2.c
	$(CC) -c thread_safe_list_version2.c

clean:
	rm -rf *.o version1 version2 version3 version4

