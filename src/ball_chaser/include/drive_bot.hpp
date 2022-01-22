#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "ball_chaser/DriveToTarget.h"

class SubscriberAndPublisher
{
public:
  SubscriberAndPublisher(ros::NodeHandle* nh);
  bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res);

private:
  ros::NodeHandle m_nh;
  ros::Publisher m_motor_command_publisher;
  ros::Subscriber m_sub;
};
