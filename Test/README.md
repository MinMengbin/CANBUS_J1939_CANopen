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
 command syntax is: COB-ID#[data0][data1]  (for details, please check referece of CANopen high-level protocol for CAN-bus)   
	
    /*
    CAN-ID is 000 which is the highest identifier for NMT (CANopen network management)
    
    [data0] is the command specifier shown below
    'OPERATIONAL': 1, 0x01
    'STOPPED': 2, 0x02
    'SLEEP': 80, 0x50
    'STANDBY': 96, 0x60
    'PRE-OPERATIONAL': 128, 0x80
    'INITIALISING': 129, 0x81
    'RESET': 129,0x81
    'RESET COMMUNICATION': 130, 0x82
    
    [data1] is the node-ID which is from 1-127 (0x01-0x7F)
    
    CAN_utils example:  device: can0
    change the node id (10, A in hex) to operational mode via NMT command
	cansend can0 000#010A
    */

   cansend terminal -> input the following command to switch node-ID of 10 to operational mode
      
      cansend can0 000#010A

   canmessage terminal then gets new messages as shown below: 
     	
	can0  000   [2]  01 0A
  	can0  18A   [8]  00 00 00 00 00 00 00 00
  	can0  28A   [8]  00 00 00 00 00 00 00 00
  	can0  38A   [8]  00 00 00 00 00 00 00 00
  	can0  48A   [8]  00 00 00 00 00 00 00 00

   can0  000   [2]  01 0A 
   (This line is from the cansend terminal cansend can0 000#010A )
  	
   can0  18A   [8]  00 00 00 00 00 00 00 00
   
   can0  28A   [8]  00 00 00 00 00 00 00 00
   
   can0  38A   [8]  00 00 00 00 00 00 00 00
   
   can0  48A   [8]  00 00 00 00 00 00 00 00
   
   (These four lines are from the node-ID of 10. They are TPDO messages which are default.)
 
 Change node-ID of 10 to pre-operational mode again, then we can test SDO protocol
 	
	cansend can0 000#800A

# SDO (Service Data Object) protocol
  
command syntax: COB-ID#[data0][data1][data2][data3][data4-data7] (for details, please check referece of CANopen high-level protocol for CAN-bus)
  
First test expedited transfer.

# (Initiate Domian Upload)
/*****************

 cansend terminal -> input the following command to upload the value of number of mappings. (client -> server)
      
      ~$ cansend can0 60A#40001A0000000000

600+A: COB-ID; 0x1A00: index; 0x00: subindex; (Refer to the EDS file of the can node to get the value of index and subindex)

canmessage terminal receives new two lines as shown below (server -> client)

      can0  60A   [8]  40 00 1A 00 00 00 00 00
      can0  58A   [8]  4F 00 1A 00 08 00 00 00
   
   can0  58A   [8]  4F 00 1A 00 08 00 00 00

(This line is from the node-ID of 10. 0x08 means there are 8 mappings.)


If you send some commands which do not contain the right index and subindex values in the object dictionary (OD), you will get Abort Domain Transfer feedback from the canbus as shown below. The first byte of data is 0x80 which means Abort Domain Transfer.
      
      ~$ cansend can0 60A#401A000300000000

      
      can0  60A   [8]  40 1A 00 01 00 00 00 00
      can0  58A   [8]  80 1A 00 01 00 00 02 06
 *****************/
 
 
# (Initiate Domian Download)
/*****************

 cansend terminal -> input the following command to upload the value of number of mappings. (client -> server)
      
      ~$ cansend can0 58A#2F001A0009

canmessage terminal receives new two lines as shown below (server -> client)

      can0  60A   [8]  40 00 1A 00 00 00 00 00
      can0  58A   [8]  4F 00 1A 00 08 00 00 00
   
 *****************/
