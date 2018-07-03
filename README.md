# CAN_CANBUS /SAE J1939 /CANopen
# NOTE： I am currently still working on it. 

# The USB2CAN device is 

https://www.ixxat.com/docs/librariesprovider8/default-document-library/products/can-pc-interfaces/usb-to-can-v2-manual-english.pdf?sfvrsn=2


# This is a C++ coding demo for CAN bus communication. It is based on SocketCAN for Linux.
The main purpose of this project is to creat an application on PC (Linux system) for CAN comminication. Based on SocketCAN for Linux, I want to build my program to communicate with other devices via either SAE J1939 or CANopen communication protocol. This program is a low-level CAN communication application. It is not based on others project such as CANopenNode.

# Important header files from system and linux:

#include <linux/can.h>

#include <sys/socket.h>

# How to automatic load driver and set up can bitrate and bring it up when USB2CAN is plugged?

First, you should complile and install IXXAT socketcan driver on Ubuntu linux system. Please download the driver from 

https://www.ixxat.com/products/products-industrial/pc-interfaces/pc-can-interfaces/socketcan

Then follow these steps from a readme file contained by this download documents. (Remeber that, you should do this again once you upgrade your Ubuntu system)

If you finish the above, you should be able to see below by run some commands.

$ udevadm info -q all -p /sys/class/net/can0
P: /devices/pci0000:00/0000:00:14.0/usb1/1-8/1-8:1.0/net/can0
E: DEVPATH=/devices/pci0000:00/0000:00:14.0/usb1/1-8/1-8:1.0/net/can0
E: ID_BUS=usb
E: ID_MM_CANDIDATE=1
E: ID_MODEL=USB-to-CAN_V2
E: ID_MODEL_ENC=USB-to-CAN\x20V2
E: ID_MODEL_ID=0008
E: ID_NET_DRIVER=ixx_usb
E: ID_NET_LINK_FILE=/lib/systemd/network/99-default.link
E: ID_PATH=pci-0000:00:14.0-usb-0:8:1.0
E: ID_PATH_TAG=pci-0000_00_14_0-usb-0_8_1_0
E: ID_REVISION=0100
E: ID_SERIAL=IXXAT_USB-to-CAN_V2_HW428439
E: ID_SERIAL_SHORT=HW428439
E: ID_TYPE=generic
E: ID_USB_DRIVER=ixx_usb
E: ID_USB_INTERFACES=:ffffff:
E: ID_USB_INTERFACE_NUM=00
E: ID_VENDOR=IXXAT
E: ID_VENDOR_ENC=IXXAT
E: ID_VENDOR_FROM_DATABASE=IXXAT Automation GmbH
E: ID_VENDOR_ID=08d8
E: IFINDEX=4
E: INTERFACE=can0
E: SUBSYSTEM=net
E: SYSTEMD_ALIAS=/sys/subsystem/net/devices/can0
E: TAGS=:systemd:
E: USEC_INITIALIZED=315431131


This device information printed above is useful if you want to add some udev rules (http://www.reactivated.net/writing_udev_rules.html). udev rules will do something you want when the USB2CAN is plugged or unplugged based on system events.


Now, the can bus (for my case it is can0) is treated as a net since the driver is based on socketcan. You can set up can0 bitrate automatically instead of by typing command " sudo ip link set can0 type can biterate 500000 ". This is done by adding a few lines to the internet configuration files at /etc/network/interfaces. Follow the steps:

cd /etc/network   # direct to network folder

sudo nano interfaces  # edit the file of interfaces

Then you will see the following lines

#interfaces(5) file used by ifup(8) and ifdown(8)
auto lo
iface lo inet loopback

add folling lines after. You should see soemthing like this

iface lo inet loopback
allow-hotplug can0
iface can0 can static
    bitrate 500000 #bitrate set as 500000
#up ip link set $IFACE txqueuelen 20 #uncomment if more than 4 nodes are used

More information about how to edit interfaces file, please refer
https://wiki.debian.org/NetworkConfiguration

# Reference:

CANopen knowledge: http://www.canopensolutions.com/index.html

CIA 301 profile document: https://www.can-cia.org/index.php?eID=tx_nawsecuredl&u=12180&g=11&t=1530140445&hash=e6dd15e451da5974a65752ae52f567f99db00d7c&file=fileadmin/resources/documents/groups/301v04020006_cor3.pdf

CANopen application layer and communication profile: https://www.can-cia.org/fileadmin/resources/documents/brochures/co_poster.pdf

SocketCAN: https://www.kernel.org/doc/Documentation/networking/can.txt

CANopen for Python: http://canopen.readthedocs.io/en/latest/nmt.html# 

SocketCAN wikipedia: https://en.wikipedia.org/wiki/SocketCAN


Introduction to J1939 from Vector: https://vector.com/portal/medien/cmc/application_notes/AN-ION-1-3100_Introduction_to_J1939.pdf

Simmasoftware Presentation about J1939: http://www.simmasoftware.com/j1939-presentation.pdf
