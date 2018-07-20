# This file is used to test canopen protocols including NMT(Network ManageMent) protocol, Boot-up protocol, Heartbeat protocol, SDO(Service Data Object) protocol, PDO(Process Data Object) protocol. 

First plug in the usb2can to the linux computer, which is the master of the canbus system.

Before powering on every canbus node which connected to the canbus, open one terminal and input the following command to monitor canbus messages
           
           ~$ candump can0 
           
Open another terminal and input cansend messages later, power on all the nodes on the canbus system. For my case, it is ABB CAN slave module CM588.

# Boot-up protocol (After power on, CM588 sends out a boot-up message and a pre-operational state message, change to pre-operational state)
      ~$ candump can0
            can0  70A   [1]  00
            can0  08A   [8]  00 00 00 00 00 00 00 00
can0 is the aliased name for the CANbus

70A is the COB-ID (Hex) or CAN-ID =  700 + A (node-ID); [1] 1 byte data; 00 value of the data




# NMT command test
      can0 

# SDO command test

      can0  60A   [8]  40 00 1A 00 00 00 00 00
      can0  58A   [8]  4F 00 1A 00 08 00 00 00

If you send some commands which do not contain the right index and subindex values in the object dictionary (OD), you will get Abort Domain Transfer feedback from the canbus as shown below. 
      
      ~$ cansend can0 60A#401A000300000000

      
      can0  60A   [8]  40 1A 00 01 00 00 00 00
      can0  58A   [8]  80 1A 00 01 00 00 02 06
