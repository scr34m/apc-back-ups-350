all : apc

apc: apc.c
	gcc -I. -I/usr/include/libusb-1.0/ -Wall -lusb-1.0 -o $@ $<

clean :
	rm -rf apc *.o
