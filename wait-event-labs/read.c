#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd, rc, nbytes = 32;
	char *buffer, *filename = "/dev/mycdev";

	if (argc > 1)
		filename = argv[1];
	if (argc > 2)
		nbytes = atoi(argv[2]);
	buffer = malloc(nbytes + 1);

	fd = open(filename, O_RDONLY);
	printf("opened file: %s,  with file descriptor = %d\n", filename, fd);
	rc = read(fd, buffer, nbytes);
	buffer[rc] = '\0';
	printf("read %d bytes which were:\n%s\n", rc, buffer);
	close(fd);
	exit(EXIT_SUCCESS);
}
