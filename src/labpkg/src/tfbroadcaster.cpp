#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char** argv){

  ros::init(argc, argv, "tfbroadcaster");
  ros::NodeHandle nh;

  tf2_ros::TransformBroadcaster broadcaster;
  geometry_msgs::TransformStamped transformStamped;
  
  while (nh.ok()){

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "base_link";
    transformStamped.child_frame_id = "myframe";
    transformStamped.transform.translation.x = 4.0;
    transformStamped.transform.translation.y = 0.0;
    transformStamped.transform.translation.z = 2.0;

    transformStamped.transform.rotation = tf::createQuaternionMsgFromYaw(0);    
    broadcaster.sendTransform(transformStamped);
    
    
  }
  return 0;
};
