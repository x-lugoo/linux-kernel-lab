
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int fd, rc;
	char *buffer = "TESTING CHAR DRIVER WITH A WRITE", *filename =
	    "/dev/mycdev";

	if (argc > 1)
		filename = argv[1];

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	rc = write(fd, buffer, strlen(buffer));
	printf("process %d wrote %d bytes to %s\n", getpid(), rc, filename);
	close(fd);
	exit(EXIT_SUCCESS);
}
