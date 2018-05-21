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

char* recursive_search(char *directory);

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

	//start = clock(); // record the start time of recursive_search()
	recursive_search(start_dir);
	//end = clock(); // record the end time of recursive_search()
	
	// calculate the run-time of recursive_search()
	//run_time = ((end - start) * 1000) / CLOCKS_PER_SEC; 
	//printf("Time: %f ms\n", run_time);

	return 0;
}

/*
This function:

*/
char* recursive_search(char *directory)
{
	// 'new_dir' contains the name of new directory during the recursive search
	//char new_dir[1024];
	char *new_dir = malloc(sizeof(char) * 1024);
	/*
	'dir_container' is an array of pointers.
	Each pointers in 'dir_container' points to a directory's name
	*/ 
	char *dir_container[200];
	int used_space = 0;

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

	/*
	The readdir() returns either:
		file/directory at current position in directory stream, or 
		NULL pointer if reached at the end of directory stream.
	*/
	while((dirent_ptr = readdir(dir_ptr)) != NULL)
	{
	    if(strcmp(dirent_ptr->d_name, current) != 0 && strcmp(dirent_ptr->d_name, parent) != 0)
	    {
	    	// 'new_dir' is the name of new directory/file
	        sprintf(new_dir, "%s/%s", directory, dirent_ptr->d_name);
	       	
	        // print out the directory name and append a ":" to the end
	       	if(dirent_ptr->d_type == DT_DIR) 
	       		printf("%s:\n", new_dir);
	       	else // print out the file name
	        	printf("%s\n", new_dir);	       

	        if(dirent_ptr->d_type == DT_DIR)
	        {	
	        	recursive_search(new_dir);
	        }
	    }
	}  

	//free(new_dir); // free allocated memory for 'new_dir'
	closedir(dir_ptr); // close the directory	
	//free(dirent_ptr); // free allocated memory for 'dirent_ptr'
	//free(dir_ptr); // free allocated memory for 'dir_ptr' 	
}
