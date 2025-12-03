obj-m += queue.o
path=/lib/modules/$(shell uname -r)
all:
	make -C $(path)/build M=$(PWD) modules
clean:
	make -C $(path)/build M=$(PWD) clean
rmv:
	sudo rmmod queue
st:
	sudo mknod /dev/wqdevicefile1 c 300 0
	sudo chmod 666 /dev/wqdevicefile1
