// written by Aleksandr Brodskiy
# include <ros/ros.h>
# include <tf2_ros/transform_listener.h>
# include <geometry_msgs/TransformStamped.h>
int run(int c, char **v)
{
  ros::init(c, v, "listener");
  ros::NodeHandle nh;
  tf2_ros::Buffer buffer;
  tf2_ros::TransformListener listener(buffer);
  geometry_msgs::TransformStamped transformObtained;
  while (nh.ok())
  {
    try
    {
      transformObtained = buffer.lookupTransform("husky_beta/base_link", "husky_alpha/base_link", ros::Time(0));
    }
    catch (tf2::TransformException &ex) 
    {
      ROS_WARN("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    ROS_INFO_STREAM("Obtained transformation " << transformObtained);
  }
  return 0;
}
int main(int argc, char **argv)
{
    return run(argc, argv);
}
