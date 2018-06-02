# Recursive File Search with 4 threads

### Usage: 
```./file_search_threaded <search term> <starting directory>``` <br />
Note: parameters in [] are optional, parameters in <> are required. The | (pipe) means "or".

### Set up 
- type `make` to build `file_search_threaded`.
- type `make clean` to remove the `file_search_threaded` executable.

### Program's functions
- Search and print out all file and directory names in <starting directory> for occurrences of <search term>.
- Directory name has a ":" appended to it so that they are visually distinguishable from files.
- The `.` and `..` will neither be used to compare to the search term nor printed out in the searching process.
- Pints out the searching time which is measured in milliseconds.

### More Information
- For detailed analysis, please take a look at the Homework-5.pdf