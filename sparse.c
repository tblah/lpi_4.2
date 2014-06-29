#include <stdio.h>		// for printf, sprintf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <sys/stat.h>	// for open
#include <sys/fcntl.h>	// for open
#include <string.h>		// for strlen
#include <unistd.h>		// for lseek

// this file will implement some code to create a sparse file to be used with cp.c
// will always create a file called sparse_file


void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

// used to write some dummy data to the current file offset. No return value because error checking is done in the function
void writeStuff(int fd, char outBuf[]) {	
	ssize_t writtenBytes = write(fd, outBuf, strlen(outBuf));

	// error checking:

	if (writtenBytes == -1) {
		printf("Error writing %s\n", outBuf);
		errExit("Writing");
	}
	else if (writtenBytes != strlen(outBuf))
		printf("Warning: %ld written out of %ld\n",(long) writtenBytes, (long) strlen(outBuf));
}

int main(int argc, char* argv[])
{
	// open for writing only, creating if the file doesnt exist and truncating to 0 length if it does exist.
	int fd = open("sparse_file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // rw-rw-r-- 
	if (fd == -1)
		errExit("Open");

	// write at the beginning of the file
	writeStuff(fd, "Beginning of file");

	// seek to 100 bytes after the end of the file
	if (lseek(fd, 100, SEEK_END) == -1)
		errExit("lseek");

	// write more
	writeStuff(fd, "Sparse bit!");

	// close file
	if (close(fd) == -1)
		errExit("close");

	exit(EXIT_SUCCESS);
}
