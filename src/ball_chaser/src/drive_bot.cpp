#include "drive_bot.hpp"

//Constructor
SubscriberAndPublisher::SubscriberAndPublisher(ros::NodeHandle* nh)
  : m_nh(*nh)
{
  m_motor_command_publisher = m_nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
}

//Callback function that executes whenever a drive_bot service is requested.
bool SubscriberAndPublisher::handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
  ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular_z:%1.2f", (float)req.linear_x, (float)req.angular_z);

  //Publishes requested linear_x and angular_z velocities to the robot wheel joints
  geometry_msgs::Twist robot_wheel_joints;
  robot_wheel_joints.linear.x = req.linear_x;
  robot_wheel_joints.angular.z = req.angular_z;
  m_motor_command_publisher.publish(robot_wheel_joints);

  //Returns a feedback message
  res.msg_feedback = "Linear speed in x set: " + std::to_string(robot_wheel_joints.linear.x) + " angular speed in z set: " + std::to_string(robot_wheel_joints.angular.z);
  ROS_INFO_STREAM(res.msg_feedback);

  return true;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "drive_bot");
  ros::NodeHandle nh;

  SubscriberAndPublisher subs_pubs(&nh);

  //Defines a /ball_chaser/command_robot service
  ros::ServiceServer service = nh.advertiseService("/ball_chaser/command_robot", &SubscriberAndPublisher::handle_drive_request, (SubscriberAndPublisher*) &subs_pubs);

  ros::spin();

  return 0;
}
