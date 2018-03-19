#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/GetPlan.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <sensor_msgs/LaserScan.h>

bool tooClose = false;
int servCounter = 0;
void scanMessageRecieved(const sensor_msgs::LaserScan&msg){
    for(int i = 1; i< msg.ranges.size(); i++){
        if (msg.ranges[i] < 0.4){
            
            tooClose = true;
            break;
        }
        else 
        {
            tooClose = false;
        }
       
        
    }
}

void serviceActivated(){
	ROS_INFO_STREAM("Services recieved goal");

}

void serviceDone(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result){
 
		ROS_INFO_STREAM("Service completed");
		ROS_INFO_STREAM("Final state " << state.toString().c_str());


}


void serviceFeedback(const move_base_msgs::MoveBaseFeedbackConstPtr& fb){
	
   if (servCounter == 10){
    
        ROS_INFO_STREAM("Service still running");
	    ROS_INFO_STREAM("Current pose (x,y) " << fb->base_position.pose.position.x<< "," << fb->base_position.pose.position.y);
            servCounter = -1;
    }
    servCounter++;

}



int main (int argc, char **argv) {

	ros::init(argc,argv, "saferandomwalk");
	ros::NodeHandle nh;

	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);	
	ros::Subscriber sub = nh.subscribe("/scan",1000,&scanMessageRecieved);
	ROS_INFO_STREAM("Waiting for server to be availabel ... ");
	tf2::Quaternion q;
    

	while (!ac.waitForServer()) { }
    ros::Rate r(1);
	ROS_INFO_STREAM("done!");
  	while (ros::ok() ) {
             
		move_base_msgs::MoveBaseGoal goal;

		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();

		q.setRPY ( 0 , 0 , ((double)(rand() % 3))/5);
		goal.target_pose.pose.position.x = ((double)(rand() % 3));

		goal.target_pose.pose.orientation = tf2::toMsg(q);



		ac.sendGoal(goal, &serviceDone,&serviceActivated,&serviceFeedback);
        ros::spinOnce();
       
        if (tooClose == true){
            ac.cancelAllGoals();
            r.sleep();
        }
 		
        r.sleep();
        


	}
 	return 0;

}
