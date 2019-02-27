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
	char bufread[100];

	if (argc > 1)
		filename = argv[1];

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		printf("open error\n");
		exit(-1);
	}
	rc = read(fd, bufread, strlen(buffer));
	printf("process %d read %d bytes to %s\n", getpid(), rc, filename);
	bufread[rc] = '\0';
	printf("read buf %s\n", bufread);
	close(fd);
	exit(EXIT_SUCCESS);
}
