obj-m+=timer.o

path = /lib/modules/$(shell uname -r)/build

all:
	make -C $(path) M=$(PWD) modules
clean:
	make -C $(path) M=$(PWD) clean