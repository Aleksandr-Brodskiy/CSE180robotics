// written by Aleksandr Brodskiy
# include <math.h>
# include <ros/ros.h>
# include <Eigen/Eigenvalues>
# include <nav_msgs/GetPlan.h>
# include <nav_msgs/Odometry.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>

double volume;
Eigen::MatrixXd matrix(3,3);
Eigen::Vector3d eigVec1, eigVec2, eigVec3;
double eigVal1, eigVal2, eigVal3;
double confidenceVal = sqrt(7.815);

void EKF(const nav_msgs::Odometry&msg)
{
	matrix(0,0) = msg.twist.covariance[14];
	matrix(0,1) = msg.twist.covariance[15];
	matrix(0,2) = msg.twist.covariance[16];
	matrix(1,0) = msg.twist.covariance[20];
	matrix(1,1) = msg.twist.covariance[21];
	matrix(1,2) = msg.twist.covariance[22];
	matrix(2,0) = msg.twist.covariance[26];
	matrix(2,1) = msg.twist.covariance[27];
	matrix(2,2) = msg.twist.covariance[28];
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(matrix);
	eigVal1 = solver.eigenvalues().x();
	eigVal2 = solver.eigenvalues().y();
	eigVal3 = solver.eigenvalues().z();
	eigVec1 = solver.eigenvectors().col(0);
	eigVec2 = solver.eigenvectors().col(1);
	eigVec3 = solver.eigenvectors().col(2);
	double R1 = confidenceVal * sqrt(eigVal1);
	double R2 = confidenceVal * sqrt(eigVal2);
	double R3 = confidenceVal * sqrt(eigVal3);
	volume = (4/3)*M_PI*R1*R2*R3;
}

int run(int c, char **v) 
{
	ros::init(c, v, "uncertainty");
	ros::NodeHandle nh;
	ros::Subscriber ekf = nh.subscribe("/odometry/filtered",10, &EKF);
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
	ros::Rate r(1);
  	while (ros::ok()) 
	{
        ros::spinOnce();
		r.sleep();
        ROS_INFO_STREAM("\n\nUncertainty Ellipsoid Volume:\t\t" << volume);
		ROS_INFO_STREAM("Matrix:");
		ROS_INFO_STREAM(matrix(0,0) << "\t" << matrix(0,1) << "\t"<<  matrix(0,2));
		ROS_INFO_STREAM(matrix(1,0) << "\t" << matrix(1,1) << "\t" <<  matrix(1,2));
		ROS_INFO_STREAM(matrix(2,0) << "\t" << matrix(2,1) << "\t" <<  matrix(2,2));
		ROS_INFO_STREAM("First Eigen Vector:\t\t" << eigVec1.transpose());
		ROS_INFO_STREAM("Second Eigen Vector:\t\t" << eigVec2.transpose());
		ROS_INFO_STREAM("Third Eigen Vector:\t\t" << eigVec3.transpose());
		ROS_INFO_STREAM("First Eigen Value:\t\t" << eigVal1);
		ROS_INFO_STREAM("Second Eigen Value:\t\t" << eigVal2);
		ROS_INFO_STREAM("Third Eigen Value:\t\t" << eigVal3);
	}
 	return 0;
}

int main(int argc, char **argv)
{
	return run(argc, argv);
}
