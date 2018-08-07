#include <stdio.h>
#include "libusb.h"

int main(void)
{
    int rc = 0;
    struct libusb_device_handle *handle = NULL;

    rc = libusb_init(NULL);
    if(rc < 0) {
        printf("\nFailed to initialise libusb\n");
        return -1;
    }

    libusb_set_debug(NULL, 3);

    handle = libusb_open_device_with_vid_pid(NULL, 0x051d, 0x2);
    if(libusb_kernel_driver_active(handle, 0) == 1)
    {
        if(libusb_detach_kernel_driver(handle, 0) != 0)
        {
            printf("\nCouldn't detach kernel driver!\n");
            rc = -1;
            goto fail;
        }
    }

    unsigned char buff[8];
    double dValue = 0;
    int j;
    int values[6] = {0x0320,0x0319,0x031e,0x030c,0x032c,0x031d};
    for (j=0;j<6;j++) {
        rc = libusb_control_transfer(handle, 0xa1, 1, values[j], 0, (unsigned char*)&buff, 8, 0);
        if(rc < 0) {
            printf("\nError in write!\n");
            rc = -1;
            goto fail;
        }
        /*
        printf("%04x: ", values[j]);
        for (i = 0; i < rc; i++) {
            printf("%02x ", buff[i]);
        }
        printf("\n");
        */
        dValue = (buff[2] << 8) + buff[1];
        if (values[j] == 0x0320) {// UPS.Input.Voltage
            printf("input.voltage: %.2f\n", dValue / 10);
        } else if (values[j] == 0x0319) { // UPS.Battery.RunTimeToEmpty
            printf("battery.runtime: %.0f\n", dValue);
        } else if (values[j] == 0x031e) { // UPS.Battery.Temperature
            printf("battery.temperature: %.2f\n", dValue / 100);
        } else if (values[j] == 0x030c) { // UPS.PowerSummary.RemainingCapacity
            printf("battery.charge: %d\n", buff[1]);
        } else if (values[j] == 0x032c) { // UPS.Output.PercentLoad
            printf("ups.load: %.2f\n", dValue / 10);
        } else if (values[j] == 0x031d) { // UPS.Battery.Voltage
            printf("battery.voltage: %.2f\n", dValue / 100);
        }
    }

fail:
    libusb_close(handle);
    libusb_exit(NULL);

    return rc;
}