obj-m := crypto_pattimura.o
crypto_pattimura-y := pattimura.o module.o utils.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
