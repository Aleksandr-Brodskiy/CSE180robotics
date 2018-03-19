#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/GetPlan.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


int servCounter = 0;
double diverge = 0.174533;
long double imuAng, amclAng;


void imuData(const sensor_msgs::Imu::ConstPtr& msg)
{
  //ROS_INFO("Imu Seq: [%d]", msg->header.seq);
  //ROS_INFO("Imu Orientation x: [%f], y: [%f], z: [%f], w: [%f]", msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);

tf2::Quaternion iq(msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);
imuAng = iq.getAngle();
}


void amclData(const geometry_msgs::PoseWithCovarianceStampedPtr& msg){
    
     // ROS_INFO("AMCL ORIENTATION X:[%f] :: Y:[%f] :: Z:[%f] :: w [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z,msg->pose.pose.orientation.w);
	  tf2::Quaternion aq(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z,msg->pose.pose.orientation.w);
	  amclAng = aq.getAngle();
}
void serviceActivated(){
	ROS_INFO_STREAM("Services recieved goal");

}

void serviceDone(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result){
 
		ROS_INFO_STREAM("Service completed");
		ROS_INFO_STREAM("Final state " << state.toString().c_str());


}


void serviceFeedback(const move_base_msgs::MoveBaseFeedbackConstPtr& fb){
/*	
   if (servCounter == 10){
    
        ROS_INFO_STREAM("Service still running");
	    ROS_INFO_STREAM("Current pose (x,y) " << fb->base_position.pose.position.x<< "," << fb->base_position.pose.position.y);
            servCounter = -1;
    }
    servCounter++;*/

}



int main (int argc, char **argv) {

	ros::init(argc,argv, "detectdivergence");
	ros::NodeHandle nh;

	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);	
	ros::Subscriber imuSub = nh.subscribe("/imu/data",1000, imuData);
    ros::Subscriber amclSub = nh.subscribe("/amcl_pose",1000, amclData);
	ROS_INFO_STREAM("Waiting for server to be available ... ");
	tf2::Quaternion q;
    int pose = 1;
    bool firstRun = true;
	ros::Time startTime = ros::Time::now();
  	ros::Duration duration = ros::Duration(10);
	while (!ac.waitForServer()) { }
    ros::Rate r(4);
	ROS_INFO_STREAM("done!");
  	while (ros::ok() ) {
		  
		move_base_msgs::MoveBaseGoal goal;
		if ( ( ros::Time::now().sec - startTime.sec ) >= duration.sec )
       {
         startTime = ros::Time::now();
		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();
       
		q.setRPY ( 0 , 0 , ((double)(rand() % 10))/10);
		goal.target_pose.pose.position.x = ((double)(rand() % 5));
        goal.target_pose.pose.position.y = ((double)(rand() % 5));
		goal.target_pose.pose.orientation = tf2::toMsg(q);
		ac.sendGoal(goal, &serviceDone,&serviceActivated,&serviceFeedback);
        
	   }
        ros::spinOnce();
        
		if (abs(imuAng - amclAng) > diverge){
			ROS_INFO_STREAM("Canceing all Goals due to Divergence" << (abs(imuAng - amclAng)));
			ac.cancelAllGoals();
			ros::shutdown();
			
		}
    	
       
        /*if (outOfSync == true){
            ac.cancelAllGoals();
            r.sleep();
        }*/
 		
        r.sleep();
        


	}
 	return 0;

}
