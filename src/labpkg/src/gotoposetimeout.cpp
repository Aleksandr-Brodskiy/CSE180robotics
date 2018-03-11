// written by Aleksandr Brodskiy
# include <cmath>
# include <cstdlib>
# include <ros/ros.h>
# include <geometry_msgs/Pose2D.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>
float x;
float y;
float theta;
bool msgs;
void gotolocation(const geometry_msgs::Pose2D&msg) 
{
    x = msg.x;
	y = msg.y;
	theta = msg.theta;
	msgs = true;
}
int run(int c, char **v)
{
    ros::init(c, v, "gotopose");
    ros::NodeHandle nh;
    ros::Subscriber subPos =nh.subscribe("targetpose", 1000, &gotolocation);
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goToGoal("move_base", true);
    while(ros::ok())
    {
        while (!goToGoal.waitForServer()) 
        {
            exit(0);
        }
        while(!msgs)
        {
            ros::spinOnce();
        }
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.orientation.z = sin(theta/2);
        // goal.target_pose.pose.orientation.w = cos(theta/2);
        goToGoal.sendGoal(goal);
        goToGoal.waitForResult();
        if (goToGoal.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
        {
         	goToGoal.waitForResult(ros::Duration(5.0));
            ROS_INFO_STREAM("\t\t\t\tGOAL Reached!\n\n");
        }
        else 
        {
	        ROS_INFO_STREAM("Failure");
            goToGoal.cancelGoal();
        }
        msgs = false;
    }
  return 0;
}
int main(int argc, char **argv)
{
    return run(argc, argv);
}

