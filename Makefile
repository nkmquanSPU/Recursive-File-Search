all:
	gcc -o file_search file_search.c
	gcc -o file_search_threaded file_search_threaded.c -pthread
clean:
	rm -rf file_search file_search_threaded

