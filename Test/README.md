# This file is used to test canopen protocols including NMT(Network ManageMent) protocol, Boot-up protocol, Heartbeat protocol, SDO(Service Data Object) protocol, PDO(Process Data Object) protocol. 

First plug in the usb2can to the linux computer, which is the master of the canbus system.

Before powering on every canbus node which connected to the canbus, open one terminal (named as canmessage terminal) and input the following command to monitor canbus messages
           
           ~$ candump can0 
           
Open another terminal (named as cansend terminal) and input cansend messages later. 

Power on all the nodes on the canbus system. For my case, it is ABB CAN slave module CM588.

# Boot-up protocol (After power on, CM588 sends out a boot-up message and a pre-operational state message, change to pre-operational state)
      ~$ candump can0
            can0  70A   [1]  00
            can0  08A   [8]  00 00 00 00 00 00 00 00
can0 is the aliasing name for the CANbus

70A: COB-ID (Hex) or CAN-ID =  700 + A (node-ID); [1] : 1 byte data; 00: value of the data

(An NMT-Slave issues the Boot-up message to in-dicate  to  the  NMT-Master  that  it  has  entered  the  state Pre-operational from state Initialising.)

# NMT command test

/*
    Regarding CANopen device, please change the status from pre-operational to operational
    after initialization.
    CAN-ID is 000 which is the highest identifier for NMT (CANopen network management)
    'OPERATIONAL': 1, 0x01
    'STOPPED': 2, 0x02
    'SLEEP': 80, 0x50
    'STANDBY': 96, 0x60
    'PRE-OPERATIONAL': 128, 0x80
    'INITIALISING': 129,
    'RESET': 129,
    'RESET COMMUNICATION': 130
    CAN_utils example:  device: can0
    change the node id (10, A in hex) to operational model via NMT command
	cansend can0 000#010A
 */

   cansend terminal -> input can 
      
      can0 

# SDO command test

      can0  60A   [8]  40 00 1A 00 00 00 00 00
      can0  58A   [8]  4F 00 1A 00 08 00 00 00

If you send some commands which do not contain the right index and subindex values in the object dictionary (OD), you will get Abort Domain Transfer feedback from the canbus as shown below. 
      
      ~$ cansend can0 60A#401A000300000000

      
      can0  60A   [8]  40 1A 00 01 00 00 00 00
      can0  58A   [8]  80 1A 00 01 00 00 02 06
