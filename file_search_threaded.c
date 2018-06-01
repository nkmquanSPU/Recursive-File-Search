#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>

#define N 4 // number of threads

//takes a file/dir as argument, recurses,
// prints name if empty dir or not a dir (leaves)
void *recur_file_search(void *arg);

//share search term globally (rather than passing recursively)
const char *search_term;

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: my_file_search <search_term> <dir>\n");
		printf("Performs recursive file search for files/dirs matching\
				<search_term> starting at <dir>\n");
		exit(1);
	}

	//don't need to bother checking if term/directory are swapped, since we can't
	// know for sure which is which anyway
	search_term = argv[1];

	//open the top-level directory
	DIR *dir = opendir(argv[2]);

	//make sure top-level dir is openable (i.e., exists and is a directory)
	if(dir == NULL)
	{
		perror("opendir failed");
		exit(1);
	}

	char dir_container[100][1024]; // contains all subdirectories in the starting directory
	int dir_container_counter = -1; // count number of directories in dir_container

	struct dirent *cur_file;
	while((cur_file = readdir(dir)) != NULL)
	{
		if((strcmp(cur_file->d_name, ".") != 0) && (strcmp(cur_file->d_name, "..") != 0))
		{
			if(cur_file->d_type == DT_DIR)
			{
				dir_container_counter++;
				sprintf(dir_container[dir_container_counter], "%s/%s", argv[2], cur_file->d_name);				
			}
		}
	}

	closedir(dir);
	
	// create an array of threads
	pthread_t my_threads[N];

	//start timer for recursive search
	struct timeval start, end;
	gettimeofday(&start, NULL);

	/*
	Each thread searches in one subdirectory of starting directory for the search term.
	Once a thread finishes searching one subdirectory, it chooses another unassigned
		subdirectory to do the searching.
	*/
	if(dir_container_counter != -1) // if there is at least 1 subdirectory
	{ 								// 	in the given starting directory
		int i;

		for(i = -1; i < dir_container_counter; )
		{
			if(i < dir_container_counter) // Thread 0
			{
				i++;								
				pthread_create(&my_threads[0], NULL, recur_file_search, dir_container[i]);
	    	}

			if(i < dir_container_counter) // Thread 1
			{
				i++;				
				pthread_create(&my_threads[1], NULL, recur_file_search, dir_container[i]); 
			}

			if(i < dir_container_counter) // Thread 2
			{
				i++;				
				pthread_create(&my_threads[2], NULL, recur_file_search, dir_container[i]);
			}
			
			if(i < dir_container_counter) // Thread 3
			{
				i++;				
				pthread_create(&my_threads[3], NULL, recur_file_search, dir_container[i]);
			}
		}
		pthread_join(my_threads[0], NULL);
		pthread_join(my_threads[1], NULL);
		pthread_join(my_threads[2], NULL);
		pthread_join(my_threads[3], NULL);
	}

	gettimeofday(&end, NULL);
	printf("Time: %ld\n", (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec));

	return 0;
}


//This function takes a path to recurse on, searching for mathes to the
// (global) search_term.  The base case for recursion is when *file is
// not a directory.
//Parameters: the starting path for recursion (char *), which could be a
// directory or a regular file (or something else, but we don't need to
// worry about anything else for this assignment).
//Returns: nothing
//Effects: prints the filename if the base case is reached *and* search_term
// is found in the filename; otherwise, prints the directory name if the directory
// matches search_term.
void *recur_file_search(void *arg)
{
	char *file;
	file = (char *) arg; // cast the arg

	//check if directory is actually a file
	DIR *d = opendir(file);

	//NULL means not a directory (or another, unlikely error)
	if(d == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{	
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", file);
			exit(1);
		}

		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(file, search_term) != NULL)
			printf("%s\n", file);

		//no need to close d (we can't, it is NULL!)
		return NULL;
	}

	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(file, search_term) != NULL)
		printf("%s/\n", file);

	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(d)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&\
				strcmp(cur_file->d_name, ".") != 0)
		{
			//we need to pass a full path to the recursive function, 
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
			char *next_file_str = malloc(sizeof(char) * \
					strlen(cur_file->d_name) + \
					strlen(file) + 2);

			strncpy(next_file_str, file, strlen(file));
			strncpy(next_file_str + strlen(file), \
					"/", 1);
			strncpy(next_file_str + strlen(file) + 1, \
					cur_file->d_name, \
					strlen(cur_file->d_name) + 1);

			//recurse on the file
			recur_file_search(next_file_str);

			//free the dynamically-allocated string
			free(next_file_str);
		}
	}

	//close the directory, or we will have too many files opened (bad times)
	closedir(d);
}
