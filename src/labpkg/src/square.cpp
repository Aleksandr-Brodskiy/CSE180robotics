// written by Aleksandr Brodskiy
# include <ros/ros.h>
# include <geometry_msgs/Twist.h>
int run(int c, char **v)
{
	ros::init(c, v, "square");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 1000);
    ros::Rate rate(1);
    geometry_msgs::Twist msg;
    msg.linear.x = 0.0;
    msg.angular.z = 0.0;
    while(ros::ok())
    {
		 ROS_INFO_STREAM("linear="<<msg.linear.x<<"angular="<<msg.angular.z);
		msg.linear.x = 1;    
		msg.angular.z = 0.0;
		pub.publish(msg);
		rate.sleep(); 
		msg.linear.x = 0;
		msg.angular.z = M_PI/2; 
		pub.publish(msg);
		rate.sleep(); 
	}
}
int main(int argc, char **argv)
{
	return run(argc, argv);
}
