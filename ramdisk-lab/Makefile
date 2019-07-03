obj-m   += sample_blk.o

#KDIR    := /lib/modules/$(shell uname -r)/build
KDIR    := /root/jeff/git/linux-next
PWD     := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f modules.order

