# include <ros/ros.h>
# include <ros/time.h>
# include <std_msgs/Time.h>
int run(int c, char **v)
{
	ros::init(c, v, "sender");
	ros::NodeHandle nh;
	ros::Publisher pubTime = nh.advertise<std_msgs::Time>("time", 1000);
	ros:: Rate rate(1);
	std_msgs::Time sendTime;
	while(ros::ok())
	{
		sendTime.data = ros::Time::now();
		pubTime.publish(sendTime);
		ROS_INFO_STREAM("TIME_SENT:  " << sendTime.data);
		rate.sleep();
	}
}
int main(int argc, char **argv)
{
	return run(argc, argv);
}
