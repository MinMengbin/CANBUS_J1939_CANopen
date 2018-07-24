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
 
 Read file Auto_driver_Set_CAN.md
 
# How to use CANopen protocol
  Please go to folder of TEST (This file is used to test canopen protocols including NMT(Network ManageMent) protocol, Boot-up protocol, Heartbeat protocol, SDO(Service Data Object) protocol, PDO(Process Data Object) protocol.)
 

# Reference:

CANopen knowledge: http://www.canopensolutions.com/index.html

CIA 301 profile document: https://www.can-cia.org/index.php?eID=tx_nawsecuredl&u=12180&g=11&t=1530140445&hash=e6dd15e451da5974a65752ae52f567f99db00d7c&file=fileadmin/resources/documents/groups/301v04020006_cor3.pdf

CANopen application layer and communication profile: https://www.can-cia.org/fileadmin/resources/documents/brochures/co_poster.pdf

SocketCAN: https://www.kernel.org/doc/Documentation/networking/can.txt

CANopen for Python: http://canopen.readthedocs.io/en/latest/nmt.html# 

SocketCAN wikipedia: https://en.wikipedia.org/wiki/SocketCAN

CANopen high-level protocol for CAN-bus: https://www.nikhef.nl/pub/departments/ct/po/doc/CANopen.pdf

Introduction to J1939 from Vector: https://vector.com/portal/medien/cmc/application_notes/AN-ION-1-3100_Introduction_to_J1939.pdf

Simmasoftware Presentation about J1939: http://www.simmasoftware.com/j1939-presentation.pdf
