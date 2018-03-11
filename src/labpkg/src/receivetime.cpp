# include <ros/ros.h>
# include <ros/time.h>
# include <std_msgs/Time.h>
void timeMessageReceived(const std_msgs::Time&msg)
{
	ROS_INFO_STREAM("TIME_RECEIVED:  " << msg.data);
}
int run(int c, char **v)
{
        ros::init(c, v, "receiver");
        ros::NodeHandle nh;
        ros::Publisher pubTime = nh.advertise<std_msgs::Time>("time", 1000);
	ros::Subscriber subTime = nh.subscribe("time", 1000, &timeMessageReceived);
	ros::spin();
}
int main(int argc, char **argv)
{
        return run(argc, argv);
}
