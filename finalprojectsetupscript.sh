

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


mkdir -p ~/CSE180robotics/src
cd ~/CSE180robotics/src
catkin_init_workspace
cd ..
catkin_make
cd ~/CSE180robotics/src
wget http://robosrv.ucmerced.edu/finalfiles.tgz
tar zxvf finalfiles.tgz
cd ~/wsfinal
source /opt/ros/kinetic/setup.bash
catkin_make
