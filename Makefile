# Copyright 2009 SYSTEMBASE all right reserved 
# for kernel 2.6
#
ifeq "$(findstring 2.6, $(shell uname -a))" "2.6"
DRIVERNAME := golden_tulip
all:	modules 

install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a

	
	
ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := sb_pci_mp.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)


modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean: clean_local


endif


clean_local:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f *.mod*
	rm -f *.ko
	rm -f .*.cmd
	rm -f $(DRIVERNAME)
	rm -f Module*
	rm -f module*
#	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).ko
#	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).ko
	rm -f sb_config


else
#for kernel 2.4
DRIVERNAME :=golden_tulip
CC=gcc -c

ARCH=$(shell uname -m | sed s/i.86/i86/)


ifeq	($(ARCH),i86)
PARMS= -DMODULE -D__KERNEL__ -Wall -Wstrict-prototypes -O2 \
-fomit-frame-pointer -I/lib/modules/$(shell uname -r)/build/include
else
PARMS= -DMODULE -D__KERNEL__ -Wall -Wstrict-prototypes -O2 \
-fomit-frame-pointer -pipe -mcmodel=kernel -I/lib/modules/$(shell uname -r)/build/include -ffixed-8
endif


ifeq	($(MAKECMDGOALS), install)
PARMS+= -DMODVERSIONS
endif


ifeq	"$(findstring SMP, $(shell uname -a))" "SMP"
PARMS+= -D__SMP__
endif

all:	golden_tulip.o

install: golden_tulip.o
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).o /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).o /lib/modules/$(shell uname -r)/misc/
	depmod -a

golden_tulip.o: sb_pci_mp.o 

	ld -r -o $@ $^

sb_pci_mp.o:
	$(CC) $(PARMS) sb_pci_mp.c

clean:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f .*.cmd
	rm -f $(DRIVERNAME).mod*
	rm -f $(DRIVERNAME).o
	rm -f $(DRIVERNAME)
	rm -f sb_config
	rm -f module*
	rm -f Module* 
#	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).o
#	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).o
	rm -f sb_config
	

endif



