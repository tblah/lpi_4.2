#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <limits.h>		// for NAME_MAX
#include <sys/stat.h>	// for open
#include <sys/fcntl.h>	// for open
#include <unistd.h>		// for close for lseek

// this file will implement a simple cp command (cp file1 flie2) which can handle sparse files


void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

void checkForOpenErrors(int fd)
{
	if (fd == -1)
		errExit("Open");
}

void closeFile(int fd) {	// wrapper for close() so that I dont need to type the error checking twice
	if (close(fd) == -1)
		errExit("Close");
}

int main(int argc, char* argv[])
{
	// deal with command line arguements
	if (argc != 3){
		printf("Usage: %s file1 file2\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// open files
	int fd1 = open(argv[1], O_RDONLY); 	// open source file readonly
	checkForOpenErrors(fd1);
	// open for writing, create if not existant, otherwise truncate
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // rw-rw-r-- 
	checkForOpenErrors(fd2);

	// read 8 bytes at a time from fd1 and then write them to fd2
	char buf[8];
	ssize_t numRead = read(fd1, buf, 8);

	while (numRead != -1) {	// -1 is error
		if (numRead == 0) { // 0 means EOF
			break;	// wrting is done so leave the loop
		}
		else {	// something was actually read so write it to the output file
			if (write(fd2, buf, numRead) < numRead)
				errExit("Not enough written");
		}

		numRead = read(fd1, buf, 8);
	}

	if (numRead == -1)
		errExit("numRead");

	// close files
	closeFile(fd1);
	closeFile(fd2);

	exit(EXIT_SUCCESS);
}
