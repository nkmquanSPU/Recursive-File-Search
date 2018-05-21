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

void recursive_search(char search_term[], char *directory);

int main(int argc, char *argv[])
{
	// if user enters invalid commmand
	if(argc != 3)
	{
		printf("Usage: ./file_search <search term> <starting directory>\n"); // display program's usage
		exit(1);
	}	

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

	struct timeval start; 
	struct timeval end;
	double run_time;

	gettimeofday(&start, NULL); // record the start time of recursive_search()
	recursive_search(search_term, start_dir);
	gettimeofday(&end, NULL); // record the end time of recursive_search()

	// calculate the run-time of recursive_search() in milliseconds
	run_time = 1000.0 * ((double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec));
	printf("\n");
	printf ("Time = %f ms\n", run_time);
	
	return 0;
}

/*
This function:

Parameter: char search_term[], char *directory
Return: None
*/
void recursive_search(char search_term[], char *directory)
{
	char* found;

	// 'new_dir' contains the name of new directory during the recursive search
	char *new_dir = malloc(sizeof(char) * 1024);


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

			// find the search term in the name of the directory/file
	    	found = strstr(dirent_ptr->d_name, search_term);

			// if the search term is found in the name of the directory
	    	if((found) && (dirent_ptr->d_type == DT_DIR))    	
		        printf("%s:\n", new_dir); // print out the directory name, and append a ":" to its name
		    else if (found) // if the search term is found in the name of the file
		    	printf("%s\n", new_dir); // print out the file name
			
			// if the current entry is a directory
    		if(dirent_ptr->d_type == DT_DIR)	        	
	        	recursive_search(search_term, new_dir); // recursively search in this directory the for the search term	        
	    }
	}  

	closedir(dir_ptr); // close the directory		
}