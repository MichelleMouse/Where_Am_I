#include "process_image.hpp"

ProcessImage::ProcessImage(ros::NodeHandle* nh)
  : m_nh(*nh)
{
  m_sub = m_nh.subscribe("/camera/rgb/image_raw", 10, &ProcessImage::process_image_callback, this);
  m_client = m_nh.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
}

//Requests a service and passes the velocities to it to drive the robot
void ProcessImage::drive_robot(float lin_x, float ang_z)
{
  ROS_INFO_STREAM("Moving the robot!");
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  if(!m_client.call(srv))
  {
    ROS_ERROR("Failed to call service to ball_chaser");
  }
}

//Continuously executes and reads the image data
void ProcessImage::process_image_callback(const sensor_msgs::Image msg)
{
  int white_pixel = 255;
  int left_limit = msg.width / 3;
  int mid_limit = 2 * (msg.width / 3);
  int position = -1;
  bool found = false;

  //Loops through each pixel in the image and checks if there's a bright white one
  for(int i = 0; i < msg.height * msg.step; i+=3)
  {
    if((msg.data[i] == white_pixel) && (msg.data[i+1] == white_pixel) && (msg.data[i+2] == white_pixel))
    {
      ROS_INFO("White pixel detected, moving the robot!");
      position = int(i/3) % msg.width;  //Saves the position of the white pixel
      found = true;
      break;
    }
  }

  if(found)
  {
    if(position < left_limit)
    {
      drive_robot(0.0f, -0.5f);  //Turn right
      found = false;
    } else if(position >= left_limit && position < mid_limit) {
      drive_robot(0.5f, 0.0f);  //Drive forward
    } else if(position >= mid_limit) {
      drive_robot(0.0f, 0.5f); //Turn right
    }

  } else {
    drive_robot(0.0f, 0.0f);  //Stop
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "process_image");
  ros::NodeHandle nh;

  ProcessImage process(&nh);

  ros::spin();

  return 0;
}
