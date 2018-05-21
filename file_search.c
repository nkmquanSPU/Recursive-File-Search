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

char* recursive_search(const char *directory);

int main(int argc, char *argv[])
{
	// if user enters invalid commmand
	if(argc != 3)
	{
		printf("Usage: ./file_search <search term> <starting directory>\n"); // display program's usage
		exit(1);
	}	

	clock_t start;
	clock_t end;
    double run_time;

	char search_term[256]; // search term
	char start_dir[256]; // starting directory

	strcpy(search_term, argv[1]); // get the search term from user
	strcpy(start_dir, argv[2]); // get the starting directory from user

	// if the provided starting directory starts with a '/', or ends with a '/'
	if((start_dir[0] != '/') || (start_dir[strlen(start_dir) - 1] == '/'))
	{
		printf("The <starting directory> must start with a \'/\', and does not end with an \'/\'\n");
		exit(1); 
	}

	start = clock(); // record the start time of recursive_search()
	recursive_search(start_dir);
	end = clock(); // record the end time of recursive_search()
	
	// calculate the run-time of recursive_search()
	run_time = ((double) ((end - start)*1000)) / CLOCKS_PER_SEC; 
	printf("Time: %f\n", run_time);
	return 0;
}

/*
This function:

*/
char* recursive_search(const char *directory)
{
	/*
	The 'dir_ptr' variable a pointer of type 'DIR'.
	'DIR' is a data type representing a directory stream.
	*/
	DIR *dir_ptr; 

	/*
	The 'dirent_ptr' represents directory entry at the current position in directory stream dir_ptr.

	struct dirent {
		ino_t  d_ino       // file serial number

		char   d_name[]    // name of directory

		unsigned char d_namlen // length of the file name, not including the terminating null character

		unsigned char d_type // type of the file. Some possible values of d_type include:
							 // DT_REG 		a regular file.							 
							 // DT_DIR 		a directory.							 							 
	}


	*/
	struct dirent* dirent_ptr = (struct dirent *) malloc(sizeof(struct dirent)); ;

	char current[] = ".";
	char parent[] = "..";

	// set the 'dir_ptr' pointer to point the directory provided by user
	dir_ptr = opendir(directory); // open the directory

	// if the directory cannot be opened
	if(dir_ptr == NULL)
	{
		printf("Cannot open %s\n", directory); // print error message
		exit(1); // exit the recursive_search() function
	}
	
	do 
	{
		/*
		The readdir() returns either:
			file/directory at current position in directory stream, or 
			NULL pointer if reached at the end of directory stream.
		*/
		dirent_ptr = readdir(dir_ptr);

		// if d_type is a directory
		if(dirent_ptr->d_type == DT_DIR)	
		{	// if d_type is neither . nor ..
			if((strcmp(dirent_ptr->d_name, current) != 0) && (strcmp(dirent_ptr->d_name, parent) != 0))
				printf("%s/%s:\n", directory, dirent_ptr->d_name); // print out its name and append a ":" to the end	
		}				
		else if(dirent_ptr->d_type == DT_REG) // if d_type is a regular file 
			printf("%s/%s\n", directory, dirent_ptr->d_name); // print out its name only		
		
	} while(dir_ptr != NULL); // while the dir_ptr is not NULL 
		
	closedir(dir_ptr); // close the directory

	// free all allocated memory
	free(dirent_ptr);
	free(dir_ptr);	
}
