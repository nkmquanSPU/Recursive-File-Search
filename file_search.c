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


void recursive_search(const char *directory);

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

	recursive_search(start_dir);
	
	return 0;
}

/*
This function:

*/
void recursive_search(const char *directory)
{
	/*
	The 'directory_ptr' variable a pointer of type 'DIR'.
	'DIR' is a data type representing a directory stream.
	*/
	DIR *directory_ptr; 

	/*
	The 'directory_entry_ptr' variable a pointer of type 'dirent' 
		which is defined in the <dirent.h> header file.

	struct dirent {
		ino_t  d_ino       // file serial number
		char   d_name[]    // name of entry
	}

	*/
	struct dirent* directory_entry_ptr;

	// set the 'directory_ptr' poiter to point the directory provided by user
	directory_ptr = opendir(directory); // open the directory

	// if the directory cannot be opened
	if(directory_ptr == NULL)
	{
		printf("Cannot open %s\n", directory); // print error message
		exit(1); // exit the recursive_search() function
	}
	else
	{	
		do // while the directory_ptr is not pointing to NULL 
		{
			/*
			The readdir(directory_ptr) function reads the next directory entry 
				from the directory specified by directory_ptr, 
				and returns its address to the directory_entry_ptr.
			*/
			directory_entry_ptr = readdir(directory_ptr);

			//if(directory_ptr == NULL ) 
				//break;

			printf( "%s\n", directory_entry_ptr->d_name );
		} while(directory_ptr != NULL); 
	}


	closedir(directory_ptr); // close the directory
}
