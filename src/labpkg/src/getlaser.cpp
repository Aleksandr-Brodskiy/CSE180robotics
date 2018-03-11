// written by Aleksandr Brodskiy
# include <ros/ros.h>
# include <std_msgs/Int32.h>
# include <std_msgs/String.h>
# include <geometry_msgs/Twist.h>
# include <sensor_msgs/LaserScan.h>
using namespace std;
void scanMessageReceived(const sensor_msgs::LaserScan &msg) 
{
    int str;
    for (int c = 0; c < sizeof(msg.ranges)/sizeof(msg.ranges[0]); c++)
        str += msg.ranges[c];
    ROS_INFO_STREAM("GET_LASER RANGE:    " << str);
}
int run(int c, char **v) 
{
	ros::init(c, v, "listener");
	ros::NodeHandle nh;
	ros::Subscriber subScan = nh.subscribe("/scan", 1000, &scanMessageReceived);
	ros::spin();
}
int main(int argc, char **argv)
{
	return run(argc, argv);
}