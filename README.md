APC Back-UPS CS 350
===

A small application to read Back-UPS CS 350 values, goal was to avoid running NUT and just for fun.

Dependency:

- libusb

Usbmon kenel module used during the development to debug communication flow.

    modprobe usbmon
    cat /sys/kernel/debug/usb/usbmon/0u
    # or use tcpdump -i usbmon0 -w /data/usblog.pcap

NUT can generate usb hid descriptor informations and communications if the device supported (like APC UPS 350).
    
    /lib/nut/usbhid-ups -D -D -D -D -D -a apc 2>&1 &> hid/nut_debug.log
