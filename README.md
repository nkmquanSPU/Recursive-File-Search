# Recursive File Search

### Usage: 
`./file_search <search term> <starting directory>`
Note: parameters in [] are optional, parameters in <> are required. The | (pipe) means "or".

### Set up 
- type `make` to build `file_search`.
- type `make clean` to remove the `file_search` executable.

### Program's functions
- Search and print out all file and directory names in <starting directory> for occurrences of <search term>.
- Directory name has a ":" appended to it so that they are visually distinguishable from files.
- The `.` and `..` will neither be used to compare to the search term nor printed out in the searching process.
- Pints out the searching time which is measured in milliseconds.