# CANBUS_CAN_CANopen
# NOTE：
I am currently still working on it. 

# This is a C++ coding demo for CAN bus communication. It is based on SocketCAN for Linux.
The main purpose of this project is to creat an application on PC (Linux system) for CAN comminication. Based on SocketCAN for Linux, I want to build my program to communicate with other devices via either SAE J1939 or CANopen communication protocol. This program is a low-level CAN communication application. It is not based on others project such as CANopenNode.

# Important header files from system and linux:

#include <linux/can.h>

#include <sys/socket.h>

# Reference:

SocketCAN https://www.kernel.org/doc/Documentation/networking/can.txt

CANopen for Python http://canopen.readthedocs.io/en/latest/nmt.html# 

SocketCAN wikipedia https://en.wikipedia.org/wiki/SocketCAN
