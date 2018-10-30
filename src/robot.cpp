/***********************************************************************************
 * A simple application which demonstrates how to operate with MTracker in ROS
 **********************************************************************************/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

#include "serial_communicator.hpp"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "mtracker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Rate loop_rate(10);

  SerialCommunicator serialComm;
  serialComm.Open();


  int count = 0;
  while (ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    //ROS_INFO("%s", msg.data.c_str());

    chatter_pub.publish(msg);

    if (serialComm.Read())
    {
        // now check the received data
        ROS_INFO("%f", serialComm.cmds.state.x);
    }


    // set new values of desired velocities
    serialComm.cmds.SetWheelsVelocities(1, 5, 5);
    serialComm.Write();

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}

