# This file shows the demo of how to use ABB can slave module (CM588) with PLC module(PM583-ETH) communicate a computer via CANBUS (CANopen)


[PLC coding (Structure Text)]

      PROGRAM PLC_PRG
      VAR
        can2asend: CAN2A_SEND;
        can2arec: CAN2A_REC;
        slotNum_can : BYTE := 1;
        id_rec :WORD := 16#20A; (* 20A is the CAN_ID for *)
      END_VAR


       (* PLC coding for can communication

        From computer ( linux system) can-utils
        
        cansend can0 000#010A (* switch can-node 10 into operational mode*)
        
        cansend can0 20A#0006000000000000 

        ----> value of CO_00 change to 6, then R0 is on, R1 is off, send 108#0001000000000000 to canbus

        cansend can0 20A#0005000000000000  

        -----> value of CO_00 change to 5, then R0 is off, R1 is on send 108#0002000000000000 to canbus
        *)


        (* cansend protocol  *)
        can2asend.EN := TRUE;
        can2asend.SLOT := slotNum_can;
        can2asend.NUM  := 1;
        can2asend.DATA  := ADR(CO_00);
        CO_00 := 0;

        (* canreceive protocol *)
        can2arec.EN := TRUE;
        can2arec.SLOT := slotNum_can;
        can2arec.ID := id_rec;
        can2arec.DATA := ADR(CI_00);

        (*actions*)
        IF CI_00 = 6 THEN
            R0 := TRUE;
            R1 := FALSE;
            CO_00 := 1;
        ELSE
           R0 := FALSE;
           R1 := TRUE;
           CO_00 := 2;
        END_IF;
        
        
 [can-utils] linux computer
 
 canmessage terminal: 
 
      candump can0 
      
      can0  000   [2]  01 0A
      can0  20A   [8]  00 06 00 00 00 00 00 00
      can0  18A   [8]  00 01 00 00 00 00 00 00
      can0  20A   [8]  00 05 00 00 00 00 00 00
      can0  18A   [8]  00 02 00 00 00 00 00 00

 cansend terminal
 
      ~$ cansend can0 000#010A
      ~$ cansend can0 20A#0006000000000000
      ~$ cansend can0 20A#0005000000000000

      
