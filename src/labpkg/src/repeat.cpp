// written by Aleksandr Brodskiy
# include <math.h>
# include <iostream>
# include <ros/ros.h>
# include <tf2/utils.h>
# include <geometry_msgs/Twist.h>
# include <geometry_msgs/Pose2D.h>
# include <tf2/LinearMath/Matrix3x3.h>
# include <tf2/LinearMath/Quaternion.h>
# include <tf2_ros/transform_listener.h>
# include <geometry_msgs/TransformStamped.h>
# include <tf2_geometry_msgs/tf2_geometry_msgs.h>
float x;
float y;
float theta;
float xPrime;
float yPrime;
float thetaPrime;
double roll;
double pitch;
double yaw;
ros::Duration delta_t;
void gotolocation(const geometry_msgs::Pose2D&msg)
{
	x = msg.x;
	y = msg.y;
	theta = msg.theta;
}
int run(int c, char **v) 
{
    ros::init(c, v, "repeat");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/husky_alpha/husky_velocity_controller/cmd_vel", 1000);
	ros::Subscriber subPos = nh.subscribe("targetpose", 1000, &gotolocation);
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    geometry_msgs::TransformStamped transformation;
	geometry_msgs::TransformStamped transformationPrime;
    geometry_msgs::Twist msg;
 	ros::spinOnce();
	ros::Rate rate(10);
    while(ros::ok()) 
	{
		try 
		{
			transformation = buffer.lookupTransform("husky_beta/base_link", "map", ros::Time(0));
			transformationPrime = transformation;
			while (transformationPrime.header.stamp == transformation.header.stamp) transformationPrime = buffer.lookupTransform("husky_beta/base_link", "map", ros::Time(0));
		} 
		catch (tf2::TransformException &ex) 
		{
			ROS_WARN("%s",ex.what());
			rate.sleep();
			continue;
		}
		x = transformation.transform.translation.x - transformationPrime.transform.translation.x;
		y = transformationPrime.transform.translation.y - transformation.transform.translation.y;
		tf2::Quaternion q2(transformationPrime.transform.rotation.x,transformationPrime.transform.rotation.y, transformationPrime.transform.rotation.z, transformationPrime.transform.rotation.w);
		tf2::Quaternion q1(transformation.transform.rotation.x,transformation.transform.rotation.y, transformation.transform.rotation.z, transformation.transform.rotation.w);
		double yaw2 = tf2::getYaw(q2);
		double yaw1 = tf2::getYaw(q1);
		yaw = yaw1 - yaw2;
		delta_t = transformationPrime.header.stamp - transformation.header.stamp;
		msg.linear.x = x/(delta_t.toSec());
		msg.angular.z = yaw/delta_t.toSec();
		pub.publish(msg);
		ROS_INFO_STREAM(msg);
    }
}
int main (int argc, char **argv)
{
	return run(argc, argv);
}
