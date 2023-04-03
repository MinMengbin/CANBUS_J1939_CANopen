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

Boot-up message: 70A: COB-ID (Hex) or CAN-ID =  700 + A (node-ID); [1] : 1 byte data; 00: value of the data

Emergency message: 08A: COB-ID (Hex) or CAN-ID =  080 + A (node-ID); [8] : 8 bytes data; 0000000000000000: value of the data. All zeros means error reset or no error.


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

The overall command for this transfer is one of :

Command Code	Meaning

0x23	Write Dictionary Object reply, expedited, 4 bytes sent

0x27	Write Dictionary Object reply, expedited, 3 bytes sent

0x2B	Write Dictionary Object reply, expedited, 2 bytes sent

0x2F	Write Dictionary Object reply, expedited, 1 bytes sent


First test expedited transfer.

# [Initiate Domain Upload]

 cansend terminal -> input the following command to upload the value of number of mappings. (client -> server)
      
      ~$ cansend can0 60A#40001A0000000000

600+A: COB-ID; 0x1A00: index; 0x00: subindex; (Refer to the EDS file of the can node to get the value of index and subindex)

canmessage terminal receives new two lines as shown below (server -> client)

      can0  60A   [8]  40 00 1A 00 00 00 00 00
      can0  58A   [8]  4F 00 1A 00 08 00 00 00
   
(This line is from the node-ID of 10. 0x08 means there are 8 mappings.)


If you send some commands which do not contain the right index and subindex values in the object dictionary (OD), you will get Abort Domain Transfer feedback from the canbus as shown below. The first byte of data is 0x80 which means Abort Domain Transfer.
      
      ~$ cansend can0 60A#401A000300000000

      
      can0  60A   [8]  40 1A 00 01 00 00 00 00
      can0  58A   [8]  80 1A 00 01 00 00 02 06

 
 
# [Initiate Domain Download]

syntax COB-ID#[data0][data1][data2][data3][data4-data7] (For details please check reference of CANopen high-level protocol for CAN-bus.)

 cansend terminal -> input the following command to upload the value of COB-ID. index 0x1800,  Subindex 01. (client -> server)
      
      ~$ cansend can0 60A#230018018A010080

canmessage terminal receives new two lines as shown below (server -> client)
	
	can0  60A   [8]  23 00 18 01 8A 01 00 00
  	can0  58A   [8]  60 00 18 01 00 00 00 00

If successful, first byte is 0x60 as shown above. Otherwise, first byte is 80. Please check error codes from reference of CANopen high-level protocol for CAN-bus.	
	
Then switch to operational model:

	~$ cansend can0 000#010A

# PDO (Process Data Object) protocol

  # [check the Receive PDO Communication parameters (For ABB canslave, default are 0x1400 to 0x143F.)]

	~$ cansend can0 60A#40001400
	
canmessage terminal receives new two lines as shown below (server -> client)

  	can0  60A   [4]  40 00 14 00
  	can0  58A   [8]  4F 00 14 00 02 00 00 00
	
02 means there are two entries of RPDO0 communication parameters.
	
	~$ cansend can0 60A#40001401 
  	
	can0  60A   [4]  40 00 14 01
  	can0  58A   [8]  43 00 14 01 0A 02 00 00

First entry is COB-ID (CAN-ID), 020A.

	~$ cansend can0 60A#40001402
	
  	can0  60A   [4]  40 00 14 02
  	can0  58A   [8]  4F 00 14 02 FE 00 00 00

Second entry is Transmission type, FE (254 decimal).

# [Check the Receive PDO Mapping parameters (Check the setup or datasheet)]
  
  note: (PDO Mapping parameters index is normally 0x200 offset after PDO Communication parameters index) 0x1400 + 0x200 = 0x1600
	
	~$ cansend can0 60A#40001600
 	 
	 can0  60A   [4]  40 00 16 00
 	 can0  58A   [8]  4F 00 16 00 08 00 00 00

08 means there are 8 entries of mapping or 8 subjects.  Check the first entry of RxPDO.

	~$ cansend can0 60A#40001601
	
	can0  60A   [4]  40 00 16 01
  	can0  58A   [8]  43 00 16 01 08 01 00 22
2200(index)01(subindex)08(8 bits data or one byte data). This mathes the setup or datasheet.

# [write a RxPDO message to can0 from computer to can slave]
	
	~$ cansend can0 20A#0000000000000011
	
	can0  20A   [8]  00 00 00 00 00 00 00 01
	
After this, the value of from 2200 (index) 01 (subindex) to 2200 (index) 08 (subindex) changed to 00 00 00 00 00 00 00 01 relatively.

# [check the Transmit PDO Communication parameters (For ABB can slave, first is 0x1800.)]
	
	~$ cansend can0 60A#40001800
	
 	can0  60A   [4]  40 00 18 00
  	can0  58A   [8]  4F 00 18 00 03 00 00 00
03 means three entries, then have a look at the entries
	
	~$ cansend can0 60A#40001801
	~$ cansend can0 60A#40001802
	~$ cansend can0 60A#40001803

  	can0  60A   [4]  40 00 18 01
 	can0  58A   [8]  43 00 18 01 8A 01 00 00  (COB-ID 018A)
  	can0  60A   [4]  40 00 18 02
  	can0  58A   [8]  4F 00 18 02 FE 00 00 00  (Transmission type, FE,254 decimal)
  	can0  60A   [4]  40 00 18 03
 	can0  58A   [8]  4B 00 18 03 00 00 00 00  (Inhibit time, 0 * 100 us = 0 us)
# [check the Transmit PDO Mapping which is similar to RxPDO]
	
	~$ cansend can0 60A#40001A00
	~$ cansend can0 60A#40001A01

	can0  60A   [4]  40 00 1A 00
  	can0  58A   [8]  4F 00 1A 00 08 00 00 00 (8 entries)
  	can0  60A   [4]  40 00 1A 01
  	can0  58A   [8]  43 00 1A 01 08 01 00 20 (2000(index)01(subindex)08(8 bits data or 1 byte data))
	
Activate or Deactivate PDOs. Using SDO download protocol to change the PDO's COB-ID value to achieve this. Because if the highest bit of COB-ID is 1, this PDO is invalid. Vise versa.) Here, I am going to change the node-ID of 10's PDO0 COB-ID value from 0000018A into 8000018A. Then this PDO0 is invalid.

	~$ cansend can0 60A#230018018A010080
  	
	can0  60A   [8]  23 00 18 01 8A 01 00 80
  	can0  58A   [8]  60 00 18 01 00 00 00 00

Check the new value 
	
	~$ cansend can0 60A#40001801
	
  	can0  60A   [4]  40 00 18 01
  	can0  58A   [8]  43 00 18 01 8A 01 00 80
	
or swith can0 to operational mode

	~$ cansend can0 000#010A

  	can0  000   [2]  01 0A
  	can0  28A   [8]  00 00 00 00 00 00 00 00
  	can0  38A   [8]  00 00 00 00 00 00 00 00
  	can0  48A   [8]  00 00 00 00 00 00 00 00
TPDO0 message (can0  18A   [8]  00 00 00 00 00 00 00 00) is gone which confirms that PDO0 is inactive.

After deactivating this PDO, then you can change these values of transmission type OD_ 1800(index)02(subindex) or inhibit time in ms  OD_ 1800(index)03(subindex). 
	
	~$ cansend can0 60A#2F001802FE
	
	can0  60A   [5]  2F 00 18 02 FE  (254)
	can0  58A   [8]  60 00 18 02 00 00 00 00

	~$ cansend can0 60A#2B0018030100
	
	can0  60A   [6]  2B 00 18 03 01 00    (inhibit time: 1 * 100 us = 100 us)
  	can0  58A   [8]  60 00 18 03 00 00 00 00

# [After these tests, please go to ABB_CAN_PLC.md to see how to use ABB can slave module CM588.]
	


