ALL: default main
obj-m   += mmap.o

KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)

main:
	gcc main.c -o main
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f modules.order main.o main

