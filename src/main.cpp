#include <iostream>
#include <bitset>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include "twist.pb.h"

int main(int argc, char** argv)
{

  ros::init(argc, argv, "ros_protobuf");

  ros_protobuf::Twist twist;
  ros_protobuf::Vector3* lin_ptr = twist.mutable_linear();
  lin_ptr->set_x(1);
  lin_ptr->set_y(2);
  lin_ptr->set_z(3);

  ros_protobuf::Vector3* ang_ptr = twist.mutable_angular();
  ang_ptr->set_x(4);
  ang_ptr->set_y(5);
  ang_ptr->set_z(6);

  std::string msg = "";
  twist.SerializeToString(&msg);
  std::cout << "got message " <<std::hex << msg << std::endl;

  ros_protobuf::Twist load_twist;
  load_twist.ParseFromString(msg);
  std::cout << "got twist " << load_twist.linear().x() << std::endl;


  geometry_msgs::Twist ros_twist;
  geometry_msgs::Vector3 ros_linear;
  ros_linear.x = load_twist.linear().x();
  ros_linear.y = load_twist.linear().y();
  ros_linear.z = load_twist.linear().z();
  ros_twist.linear = ros_linear;

  ros::NodeHandle nh("~");
  ros::Publisher pub_twist = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  ros::Rate r(10);
  while (ros::ok())
  {
    pub_twist.publish(ros_twist);
    r.sleep();
    ros::spinOnce();
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
