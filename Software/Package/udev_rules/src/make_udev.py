#!/usr/bin/env python
import glib
import gudev
import sys
import signal
import io
import os

def on_uevent_calback(client, action, device, user_data):
    vendor_name = device.get_property("ID_VENDOR_ENC")
    device_id = device.get_property("ID_SERIAL_SHORT")
    model_id = device.get_property("ID_MODEL_ID")
    vendor_id = device.get_property("ID_VENDOR_ID")

    if action == "add":
        global udev_rules

        print "Device information:"
        print "vendor name = " + vendor_name
        print "device id   = " + str(device_id)
        print "model id    = " + model_id
        print "vendor id   = " + vendor_id

        sym_link = "bcu" # port name for tictactoe base control unit
        udev_rules_string = "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"%s\", ATTRS{idProduct}==\"%s\", ATTRS{serial}==\"%s\", MODE=\"0666\" SYMLINK+=\"%s\"\r\n" %(vendor_id, model_id, device_id, sym_link)
        udev_rules += udev_rules_string

        print("\nNew udev rules is successfully created.")
        print("Press CTRL+C to save the new udev rules.\r\n")



if __name__ == "__main__":

    print("Plug in your usb device\r\n")
    client = gudev.Client(["usb/usb_device"])
    client.connect("uevent", on_uevent_calback, None)
    udev_rules=""


    try:
        loop = glib.MainLoop()
        loop.run()

    except KeyboardInterrupt:
        while True:
            save_rules = raw_input(" Do you want to save the new udev_rules?(Y/N) : ")
            if save_rules.upper() == 'Y':
                if len(udev_rules) > 0:
                    with io.FileIO("25-tictactoe.rules", "w") as file:
                        file.write(udev_rules)
                    break
                else:
                    break

            elif save_rules.upper() == 'N':
                break
            else:
                pass
sys.exit()
