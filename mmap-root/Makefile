obj-m   += mmap-root.o kmalloc.o

KDIR    := /lib/modules/`uname -r`/build
PWD     := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	gcc ./jeff-mmap.c -o jeff-mmap

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f modules.order
	rm -f ./jeff-mmap

