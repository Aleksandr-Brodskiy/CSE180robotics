// written by Aleksandr Brodskiy
# include <ros/ros.h>
# include <sensor_msgs/Imu.h>
# include <nav_msgs/GetPlan.h>
# include <geometry_msgs/Pose2D.h>
# include <tf2/LinearMath/Quaternion.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <tf2_geometry_msgs/tf2_geometry_msgs.h>
# include <actionlib/client/simple_action_client.h>
# include <geometry_msgs/PoseWithCovarianceStamped.h>

double angleIMU; 
double angleAMCL;
int pose = 1;

void imuData(const sensor_msgs::Imu::ConstPtr &msg)
{
	tf2::Quaternion iq(msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);
	angleIMU = iq.getAngle();
}

void amclData(const geometry_msgs::PoseWithCovarianceStampedPtr &msg)
{
	  tf2::Quaternion aq(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z,msg->pose.pose.orientation.w);
	  angleAMCL = aq.getAngle();
}

int run(int c, char **v) 
{
	ros::init(c, v, "detectdivergence");
	ros::NodeHandle nh;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goToGoal("move_base", true);	
	ros::Subscriber imuSub = nh.subscribe("/imu/data",1000, imuData);
    ros::Subscriber amclSub = nh.subscribe("/amcl_pose",1000, amclData);
	tf2::Quaternion q;
	ros::Time startTime = ros::Time::now();
  	ros::Duration duration = ros::Duration(10);
    ros::Rate rate(10);
  	while (ros::ok() ) 
	{
		move_base_msgs::MoveBaseGoal goal;
		if ((ros::Time::now().sec - startTime.sec) >= duration.sec)
		{
	        startTime = ros::Time::now();
			goal.target_pose.header.frame_id = "map";
			goal.target_pose.header.stamp = ros::Time::now();
       
			q.setRPY ( 0 , 0 , ((double)(rand() % 7))/7);
			goal.target_pose.pose.position.x = ((double)(rand() % 7));
	        goal.target_pose.pose.position.y = ((double)(rand() % 7));
			goal.target_pose.pose.orientation = tf2::toMsg(q);
			goToGoal.sendGoal(goal);
		}
		ros::spinOnce();
		if (abs(angleIMU - angleAMCL) > 0.174533)
		{
			ROS_INFO_STREAM("\n\t\tDivergence Detected\n\n" << (abs(angleIMU - angleAMCL)));
			goToGoal.cancelAllGoals();
			// exit(0);
		}
        rate.sleep();
	}
 	return 0;
}

int main(int argc, char **argv)
{
	return run(argc, argv);
}
