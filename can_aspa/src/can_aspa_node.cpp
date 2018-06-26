/*
 * can_aspa_node.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: aspa1
 */

// header files for standard input and output, standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ros/ros.h"// header file for ros

// header file for ros data type
#include "std_msgs/Float64.h"  //float data type


// header file for CAN frame structure and sockaddr structure and socket com
#include <linux/can.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <net/if.h>  //header file for ifreq structure




// ROS callback function for subscribing values of the target asparagus positions

// The data type of float32 is used for these values
void AspaPosub(const std_msgs::Float64& Pt_aspa)
{
	ROS_INFO("I heard: [%f]", Pt_aspa); // for debugging
    ROS_INFO(" |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| ");
}


int main(int argc, char **argv)
{
    // CAN bus handler and structure
    int s; // create a handler of CAN communication
    int nbytes; // byte numbers of received and sent messages
    //define the structure used for CAN bus
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct timeval tv;


    //set up ros
	ros::init(argc, argv, "can_aspa_node");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::Float64>("data", 1000);

	// pos_aspa is the topic for subscribe the position data
	ros::Subscriber sub = n.subscribe("pos_aspa",1000, AspaPosub);

	ros::Rate loop_rate(1); // running frequency


	// CAN bus communication initialization.
	ROS_INFO("Trying to create CAN bus connection.");

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW); // create a CAN communication which is CAN_RAW

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr)); //binding can (s) to socket
    ROS_INFO("CAN communication connected.");

    // Subscribe target asparagus position values,
    // Process target asparagus position values,
    // Communicate with PLC system

    /* Regarding CANopen device, please change the status from pre-operational to operational
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
   	// change the state operational mode
    	frame.can_id = 0x000; // NMT command
//        strcpy((char*)frame.data,"hello");
    	frame.data[0] = 0x1; // state command
    	frame.data[1] = 0xA; // node ID which is the PLC node
        frame.can_dlc = 2; // data length
        write(s,&frame, sizeof(frame));


        // read can message from PLC
		nbytes = read(s, &frame, sizeof(struct can_frame));
/*
		if (ioctl(s,SIOCGSTAMP, &tv) == -1){
		   ROS_INFO ("Getting time stamp wrongly"); // for debug
		} else
		{
			ROS_INFO("%d.%d",tv.tv_sec,tv.tv_usec);

		}
		ROS_INFO ("nbytes = %d", nbytes); // for debug


		if (nbytes < 0) {
				perror("can raw socket read fail");
				return 1;
		}

		if (nbytes < sizeof(struct can_frame)) {
				fprintf(stderr, "read: incomplete CAN frame\n");
				return 1;
		}
*/

		//chatter_pub.publish(accel_z);
		//chatter_pub.publish(Int_data);

    while ( ros::ok()) {


    	ROS_INFO("this a test point.");
    	ros::spinOnce();
    	loop_rate.sleep();

	}
   	// change to pre-operational mode
    	frame.can_id = 0x000; // NMT command
//        strcpy((char*)frame.data,"hello");
    	frame.data[0] = 0x80; // state command, stop
    	frame.data[1] = 0xA; // node ID which is the PLC node
        frame.can_dlc = 2; // data length
        write(s,&frame, sizeof(frame));
    // close CAN communication if program ends
	close(s);
	printf ("CAN communication disconnected\n");


	return 0;
}


