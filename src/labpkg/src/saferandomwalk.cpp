// written by Aleksandr Brodskiy
# include <ros/ros.h>
# include <nav_msgs/GetPlan.h>
# include <geometry_msgs/Pose2D.h>
# include <sensor_msgs/LaserScan.h>
# include <tf2/LinearMath/Quaternion.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <tf2_geometry_msgs/tf2_geometry_msgs.h>
# include <actionlib/client/simple_action_client.h>

bool obstacleProximity = false;
int counter = 0;

void scanMessageRecieved(const sensor_msgs::LaserScan&msg)
{
    for (int c = 0; c < (msg.ranges.size() - 1); c++)
    {
        if (msg.ranges[c] < 0.4)
        {
            obstacleProximity = true;
            break;
        }
        else 
        {
            obstacleProximity = false;
        }
    }
}

void actionHandlerAssign()
{
	ROS_INFO_STREAM("\n\t\tRandom GOAL Assigned\n\n");
}

void actionHandlerReach(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result)
{
		ROS_INFO_STREAM("\n\t\tRandom GOAL Reached\n\n");
}

void actionHandlerLive(const move_base_msgs::MoveBaseFeedbackConstPtr& fb)
{
   if (counter == 10)
   {
	    ROS_INFO_STREAM("\n\t\tCurrent position:\t(x: " << fb->base_position.pose.position.x << ", y: " << fb->base_position.pose.position.y << ")\n\n");
        counter = -1;
    }
    counter++;
}

int run (int c, char **v) 
{
	ros::init(c, v, "saferandomwalk");
	ros::NodeHandle nh;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goToGoal("move_base", true);	
	ros::Subscriber sub = nh.subscribe("/scan",1000,&scanMessageRecieved);
	tf2::Quaternion quaternion;
    ros::Rate rate(1);
  	while (ros::ok()) 
    {
		move_base_msgs::MoveBaseGoal goal;
		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();
		quaternion.setRPY ( 0 , 0 , ((double)(rand() % 7))/3);
		goal.target_pose.pose.position.x = ((double)(rand() % 7));
		goal.target_pose.pose.orientation = tf2::toMsg(quaternion);
		goToGoal.sendGoal(goal, &actionHandlerReach, &actionHandlerAssign, &actionHandlerLive);
        ros::spinOnce();
        if (obstacleProximity)
        {
            ROS_INFO_STREAM("\n\t\tObstaclce within 0.2[m]\n\n");
            goToGoal.cancelAllGoals();
            rate.sleep();
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