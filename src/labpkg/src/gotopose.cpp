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
    ros::Subscriber subPos = nh.subscribe("targetpose", 1000, &gotolocation);
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>goToGoal("move_base", true);
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
        goal.target_pose.pose.orientation.w = cos(theta/2);
        goToGoal.sendGoal(goal);
        goToGoal.waitForResult();
        if (goToGoal.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO_STREAM("\t\t\t\tGOAL Reached!!!!");
        else
            ROS_INFO_STREAM("\t\t\t\tFailure");
        msgs = false;
    }
    return 0;
}

int main(int argc, char **argv)
{
    return run(argc, argv);
}

// # include "tf/tf.h"
// # include <ros/ros.h>
// # include <tf/tfMessage.h>
// # include <geometry_msgs/Twist.h>
// # include <geometry_msgs/Pose2D.h>
// # include <tf/transform_listener.h>
// # include <tf2_ros/transform_listener.h>
// # include <geometry_msgs/TransformStamped.h>
// # include <tf2_geometry_msgs/tf2_geometry_msgs.h>
// float x;
// float y;
// float theta;
// float xPrime;
// float yPrime;
// float thetaPrime;
// double roll; 
// double pitch;
// double yaw;
// void gotolocation(const geometry_msgs::Pose2D&msg)
// {
// 	x = msg.x;
// 	y = msg.y;
// 	theta = msg.theta;
// }

// void readLocation(const tf::tfMessage&msg)
// {
// 	ROS_INFO_STREAM(xPrime<<" "<<yPrime<<" "<<thetaPrime);
// 	for (int c = 0; c < msg.transforms.size();c++)
// 	{
//     	if (msg.transforms[c].header.frame_id == "odom")
// 		{
//       		tf::Matrix3x3 Rotation;
//       		Rotation.setRotation(tf::Quaternion(msg.transforms[c].transform.rotation.x,
//          	msg.transforms[c].transform.rotation.y, msg.transforms[c].transform.rotation.z,
//          	msg.transforms[c].transform.rotation.w));
//       		tf::Vector3 traslation;
//       		traslation = tf::Vector3(msg.transforms[c].transform.translation.x,
//         	msg.transforms[c].transform.translation.y,
//         	msg.transforms[c].transform.translation.z);
// 			xPrime = msg.transforms[c].transform.translation.x;
// 			yPrime = msg.transforms[c].transform.translation.y;
// 			thetaPrime = msg.transforms[c].transform.rotation.z;
//       		Rotation.getEulerYPR(yaw, pitch, roll); 
//       		tf::Matrix3x3 RotationYPR;
//       		RotationYPR.setEulerYPR(yaw,pitch,roll);
//     	}
//   	}
// }

// int run(int c, char **v)
// {
//      ros::init(c, v, "gotopose");
//      ros::NodeHandle nh;
//      ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 1000);
//      ros::Subscriber subPos = nh.subscribe("targetpose", 1000, &gotolocation);
// 		ros::Subscriber subTF = nh.subscribe("/tf", 1000, &readLocation);
// 		ros::Rate rate(10);
// 		tf2_ros::Buffer buffer;
// 		tf2_ros::TransformListener listener(buffer);
// 		geometry_msgs::TransformStamped transformStamped;
// 		geometry_msgs::Twist msg;
//      while(nh.ok())
// 		{
// 			ros::spinOnce();
// 			try
// 			{
// 				transformStamped = buffer.lookupTransform("base_link","odom",ros::Time(0));		
// 				ROS_INFO_STREAM("TRANSFORM ORIGIN: " << transformStamped.transform.translation.x << "\n");
// 				ROS_INFO_STREAM("TRANSFORM ORIGIN: " << transformStamped.transform.translation.y << "\n");
// 				xPrime = transformStamped.transform.translation.x;
// 				yPrime = transformStamped.transform.translation.y;

// 				tf::Quaternion q(transformStamped.transform.rotation.x, transformStamped.transform.rotation.y, transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
//   				tf::Matrix3x3 m(q);
//   				double roll, pitch, yaw;
//   				m.getRPY(roll, pitch, yaw);
// 				thetaPrime = yaw;
				
// 				ROS_INFO_STREAM("TRANSFORM ROTATION: " << theta << "\n");
// 				// ROS_INFO_STREAM("TRANSFORM ROTATION: " << transformStamped.transform.getRotation() << "\n\n");
// 			}
// 			catch(tf2::TransformException &ex)
// 			{
// 				ROS_WARN("%s", ex.what());
// 				ros::Duration(1.0).sleep();
// 				continue;
// 			}
// 			float diff = thetaPrime - atan2(yPrime - y, xPrime - x);
// 			float dist = hypot(xPrime - x, yPrime - y);
// 			ROS_INFO_STREAM("DIFF: " << diff);
// 			ROS_INFO_STREAM("DIST: " << dist);
// 			if (abs(diff) > 0.2)
// 			{
// 				msg.linear.x = 0.0;
// 				ROS_INFO_STREAM("ROTATION\n\n");
// 				if (diff < 0) msg.angular.z = -0.2;						
// 				else msg.angular.z = 0.2;
// 			}
// 			else if (dist >= .1)
// 			{
// 				ROS_INFO_STREAM("TRANSLATION\n\n");
// 				msg.linear.x = 0.2;
// 				msg.angular.z = 0.0;
// 			}
// 			else 
// 			{
// 				ROS_INFO_STREAM("\n\n\n\t\t\t\tGOAL REACHED!!\n\n");
// 				msg.linear.x = 0;
// 				msg.angular.z = 0;
// 			}
// 			pub.publish(msg);
// 			rate.sleep();
//     	}
// 	return 0;
// }
// int main(int argc, char **argv)
// {
// 	return run(argc, argv);
// }
