// written by Aleksandr Brodskiy
# include <math.h>
# include <ros/ros.h>
# include <Eigen/Eigenvalues>
# include <nav_msgs/GetPlan.h>
# include <nav_msgs/Odometry.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>

double volume;
Eigen::MatrixXd m(3,3);
Eigen::Vector3d e1, e2,e3;
double l1, l2, l3;
double confidence = sqrt(7.815);

void ekfpose(const nav_msgs::Odometry&msg){
	//getting the 3x3 matrix within the covarience matrix
	m(0,0) = msg.twist.covariance[14];
	m(0,1) = msg.twist.covariance[15];
	m(0,2) = msg.twist.covariance[16];
	m(1,0) = msg.twist.covariance[20];
	m(1,1) = msg.twist.covariance[21];
	m(1,2) = msg.twist.covariance[22];
	m(2,0) = msg.twist.covariance[26];
	m(2,1) = msg.twist.covariance[27];
	m(2,2) = msg.twist.covariance[28];
	//Setting up the Eign Solver
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(m);
	//getting the eigen values for each collum
	l1 = solver.eigenvalues().x();
	l2 = solver.eigenvalues().y();
	l3 = solver.eigenvalues().z();
	//getting the eigen Vectors not needed but figured I'd grab the data anyway
	e1 = solver.eigenvectors().col(0);
	e2 = solver.eigenvectors().col(1);
	e3 = solver.eigenvectors().col(2);


	
	double R1 = confidence * sqrt(l1);
	double R2 = confidence * sqrt(l2);
	double R3 = confidence * sqrt(l3);
	volume = (4/3)*M_PI*R1*R2*R3;
}
int run(int c, char **v) 
{
	ros::init(c, v, "uncertainty");
	ros::NodeHandle nh;
	ros::Subscriber ekf = nh.subscribe("/odometry/filtered",10, &ekfpose);
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
	ros::Rate r(1);
  	while (ros::ok() ) 
	{
        ros::spinOnce();
		r.sleep();
        ROS_INFO_STREAM("Uncertainty Ellipsoid Volume = " << volume);
		ROS_INFO_STREAM("Covariance Matrix");
		ROS_INFO_STREAM("Row 1 " << m(0,0) << " " << m(0,1) <<" "<<  m(0,2));
		ROS_INFO_STREAM("Row 2 "<< m(1,0) << " " << m(1,1) <<" "<<  m(1,2));
		ROS_INFO_STREAM("Row 3 "<< m(2,0) << " " << m(2,1) <<" "<<  m(2,2));
		ROS_INFO_STREAM("Eigen Vector 1" << e1.transpose());
		ROS_INFO_STREAM("Eigen Vector 2" << e2.transpose());
		ROS_INFO_STREAM("Eigen Vector 3" << e3.transpose());
		ROS_INFO_STREAM("Eigen Value 1" << l1);
		ROS_INFO_STREAM("Eigen Value 2" << l2);
		ROS_INFO_STREAM("Eigen Value 3" << l3);
		ROS_INFO_STREAM("------------------------");
	}
 	return 0;
}

int main(int argc, char **argv)
{
	return run(argc, argv);
}
