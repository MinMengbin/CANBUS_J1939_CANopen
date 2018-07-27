/*
 * can_aspa_node.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: aspa1
 * for more information about can/canopen/j1939 refer to
 * https://github.com/MinMengbin/CANBUS_J1939_CANopen
 */

// header files for standard input and output, standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// can message package
#include "can_msgs/Frame.h"

// header file for ros
#include "ros/ros.h"
#include "std_msgs/Float64.h"


// variables defined for
int32_t canNodeNumber = 10; // can node number for PLC


ros::Publisher can_pub;
uint8_t lastSDOResponse[8] = {0};

uint16_t errorCode;


void AspaPosub(const std_msgs::Float64& Pt_aspa);
void canRXCallback(const can_msgs::Frame msg);
void SDO_Download(uint32_t objectIndex, uint32_t objectSubIndex, uint32_t data, uint32_t dataLength);
void Write_PDO(uint8_t pdoNumber, uint8_t dataLength, uint8_t data[]);
void CAN_Restart();
void CAN_Stop();
void CAN_Operation();


int main(int argc, char **argv)
{

    //set up ros
	ros::init(argc, argv, "can_aspa_node");

	ros::NodeHandle n_aspap;
	// pos_aspa is the topic for subscribe the position data
	ros::Subscriber aspa_sub = n_aspap.subscribe("pos_aspa",1000, AspaPosub);

    // node handle for can receive
	ros::NodeHandle n_CanRX;
	ros::Subscriber can_sub = n_CanRX.subscribe("received_messages", 100, canRXCallback);

	// node handle for can send
	ros::NodeHandle n_CanTX;
	can_pub = n_CanTX.advertise<can_msgs::Frame>("sent_messages", 100);

	ros::Rate loop_rate(1); // running frequency if needed

/*
    - Subscribe target asparagus position values,
    - Process target asparagus position values,
    - Communicate with PLC system
*/

	CAN_Restart();
	//ros::spinOnce();

/*	// Setting or mapping PDO  if needed by SDO protocol

	// Deactivate TPDO0
	SDO_Download(0x1800, 0x01, 0x80000180 + canNodeNumber, 4);

	// Activate TPDO0
	SDO_Download(0x1800, 0x01, 0x00000180 + canNodeNumber, 4);*/

	CAN_Operation();
	ros::spinOnce();


    while ( ros::ok()) {

    	// Turn Relay0 on, Turn Relay1 off
    	uint8_t PDO_data[16] = {0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/*    	PDO_data[0] = 0x01;
    	PDO_data[1] = 0x00;
    	PDO_data[2] = 0x00;
    	PDO_data[3] = 0x00;
    	PDO_data[4] = 0x00;
    	PDO_data[5] = 0x00;
    	PDO_data[6] = 0x00;
    	PDO_data[7] = 0x00;*/

    	Write_PDO(1, 8, PDO_data);
   		usleep(1000000);
        ros::spinOnce();

        // Turn Relay0 on, Turn Relay1 off
        PDO_data[1] = 0x00;;
    	Write_PDO(1, 8, PDO_data);
   		usleep(1000000);
        ros::spinOnce();

    	//loop_rate.sleep();
	}


	CAN_Stop();
	printf ("CAN communication disconnected\n");

	return 0;
}
/** ROS callback function for subscribing values of the target asparagus positions
The data type of float32 is used for these values
**/
void AspaPosub(const std_msgs::Float64& Pt_aspa)
{
	ROS_INFO("I heard: [%f]", Pt_aspa); // for debugging
    ROS_INFO(" |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ");
}

void canRXCallback(const can_msgs::Frame msg) {

	ROS_INFO("I heard CAN_ID is: [%.X]", msg.id);
	ROS_INFO("I heard [%d] bytes data: ",msg.dlc);
	for(int i = 0; i < msg.dlc; ++i){
		printf("[%.2X] ", msg.data[i]);
	}
	printf("\n");


	if (msg.id == (0x580 + canNodeNumber)) {
		for (int i = 0; i < 8; i++) {
			lastSDOResponse[i] = msg.data[i];
		}
	}
	else if (msg.id == (0x180 + canNodeNumber)) {
		// TPDO0
		ROS_INFO("TPDO0");
	}
	else if (msg.id == (0x280 + canNodeNumber)) {
		// TPDO1
		ROS_INFO("TPDO1");
	}	else if (msg.id == (0x380 + canNodeNumber)) {
		// TPDO2
		ROS_INFO("TPDO2");
	}
	else if (msg.id == (0x480 + canNodeNumber)) {
		// TPDO3
		ROS_INFO("TPDO3");
	}
	else if (msg.id == (0x80 + canNodeNumber)) {
		errorCode = msg.data[0] | (msg.data[1] << 8);
		// ROS_INFO("Error code %04X", errorCode);
	}
}

// Expedited SDO protocol
void SDO_Download(uint32_t objectIndex, uint32_t objectSubIndex, uint32_t data, uint32_t dataLength) {

	can_msgs::Frame canFrame;
	canFrame.id = 0x600 + canNodeNumber;
	canFrame.dlc = 4 + dataLength;

	// canFrame.data[0] is SDO Command Specifier (CS)
	switch (dataLength) { //initiate Domain Download
		case 1 :
			canFrame.data[0] = 0x2F; // 1 byte data
			break;
		case 2 :
			canFrame.data[0] = 0x2B; // 2 bytes data
			break;
		case 4 :
			canFrame.data[0] = 0x23; // 4 bytes data
			break;
		default :
			canFrame.data[0] = 0x22; // data size not indicated
			break;
	}

	canFrame.data[1] = objectIndex & 0x00FF;
	canFrame.data[2] = (objectIndex >> 0x08) & 0xFF;
	canFrame.data[3] = objectSubIndex & 0xFF;
	canFrame.data[4] = data & 0xFF;
	canFrame.data[5] = (data >> 0x8) & 0xFF;
	canFrame.data[6] = (data >> 0x10) & 0xFF;
	canFrame.data[7] = (data >> 0x18) & 0xFF;

	// Loop sending the packet and waiting for the node to respond correctly
	while (ros::ok()) {
		can_pub.publish(canFrame);
		// ROS_INFO("Packet sent");

		// Let ROS get any new CAN frames to the program
		// ros::spinOnce();
		usleep(50000);
		ros::spinOnce();

		// Check to see if the node has responded correctly to the SDO write
		// Data byte 0 should be 0x60, bytes 1-3 should be the same as the sent packet
		uint8_t packetsMatch = 1;
		if (lastSDOResponse[0] != 0x60) {
			packetsMatch = 0;
		}
		for (int i = 1; i < 4; i++) {
			if (lastSDOResponse[i] != canFrame.data[i]) {
				packetsMatch = 0;
				break;
			}
		}
		if (packetsMatch) {
			ROS_INFO("SDO write complete");
			return;
		}
	}
}

// PDO write
void Write_PDO(uint8_t pdoNumber, uint8_t dataLength, uint8_t data[]) {
	can_msgs::Frame canFrame;
	canFrame.id = ((pdoNumber + 1) * 0x100) + canNodeNumber;
	canFrame.dlc = dataLength;
	for (int i = 0; i < dataLength; i++) {
		canFrame.data[i] = data[i];
	}
	can_pub.publish(canFrame);
	// ROS_INFO("PDO written");
}

// Initialize can bus
void CAN_Restart() {

	// NMT protocol
	can_msgs::Frame canFrame;
	canFrame.id = 0x000;

	// switch to stopped mode
	for (int i = 0; i <= 2; ++i){
	canFrame.dlc = 2;
	canFrame.data[0] = 0x02;
	canFrame.data[1] = canNodeNumber;
	}
	can_pub.publish(canFrame);
	usleep(500000);

	// switch into pre-operational mode
	canFrame.data[0] = 0x80;
	canFrame.data[1] = canNodeNumber;

	can_pub.publish(canFrame);
	usleep(500000);

}

// Initialize can bus
void CAN_Stop() {

	// NMT protocol
	can_msgs::Frame canFrame;
	canFrame.id = 0x000;

	// switch to stopped mode
	for (int i = 0; i <= 2; ++i){
	canFrame.dlc = 2;
	canFrame.data[0] = 0x02;
	canFrame.data[1] = canNodeNumber;
	}
	can_pub.publish(canFrame);
	usleep(500000);
}

void CAN_Operation() {

	// NMT protocol
	can_msgs::Frame canFrame;
	canFrame.id = 0x000;

	// switch to stopped mode
	for (int i = 0; i <= 2; ++i){
	canFrame.dlc = 2;
	canFrame.data[0] = 0x01;
	canFrame.data[1] = canNodeNumber;
	}
	can_pub.publish(canFrame);
	usleep(500000);
}
