#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "ball_chaser/DriveToTarget.h"

class ProcessImage
{
public:
  ProcessImage(ros::NodeHandle* nh);
  void drive_robot(float lin_x, float ang_z);
  void process_image_callback(const sensor_msgs::Image msg);

private:
  ros::NodeHandle m_nh;
  ros::ServiceClient m_client;
  ros::Subscriber m_sub;
};
