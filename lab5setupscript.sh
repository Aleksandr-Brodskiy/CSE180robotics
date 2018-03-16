rm -rf ~/CSE180robotics/src/husky-kinetic-devel

sudo apt-get update
sudo apt-get install ros-kinetic-multimaster-launch
sudo apt-get install ros-kinetic-lms1xx 
sudo apt-get install ros-kinetic-hector-gazebo
sudo apt-get install ros-kinetic-interactive-marker-twist-server 
sudo apt-get install ros-kinetic-twist-mux
sudo apt-get install ros-kinetic-imu-tools
sudo apt-get install ros-kinetic-controller-manager
sudo apt-get install ros-kinetic-message-to-tf ros-kinetic-multimaster-launch ros-kinetic-lms1xx ros-kinetic-hector-gazebo ros-kinetic-interactive-marker-twist-server ros-kinetic-twist-mux ros-kinetic-imu-tools ros-kinetic-controller-manager

sudo apt-get install ros-kinetic-gazebo-ros ros-kinetic-gazebo-ros-control
sudo apt-get install ros-kinetic-gazebo-ros ros-kinetic-joint-state-controller
sudo apt-get install ros-kinetic-diff-drive-controller


cd ~/CSE180robotics/src
wget http://robosrv.ucmerced.edu/lab5.tgz
tar zxvf lab5.tgz
cd ~/CSE180robotics
rm -rf devel
rm -rf build
source /opt/ros/kinetic/setup.bash
catkin_make
