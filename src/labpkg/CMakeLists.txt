cmake_minimum_required(VERSION 2.8.3)
project(labpkg)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  geometry_msgs
  tf2
  tf2_ros
)
catkin_package()
include_directories(  ${catkin_INCLUDE_DIRS})

# add_executable(sendtime src/sendtime.cpp)
# add_executable(receivetime src/receivetime.cpp)
# add_executable(square src/square.cpp)
# add_executable(getlaser src/getlaser.cpp)
# add_executable(getpose src/getpose.cpp)
# add_executable(gotopose src/gotopose.cpp)
# add_executable(listener src/listener.cpp)
# add_executable(repeat src/repeat.cpp)
# add_executable(gotoposetimeout src/gotoposetimeout.cpp)
add_executable(saferandomwalk src/saferandomwalk.cpp)
add_executable(detectdivergence src/detectdivergence.cpp)

# target_link_libraries(sendtime ${catkin_LIBRARIES})
# target_link_libraries(receivetime ${catkin_LIBRARIES})
# target_link_libraries(square ${catkin_LIBRARIES})
# target_link_libraries(getlaser ${catkin_LIBRARIES})
# target_link_libraries(getpose ${catkin_LIBRARIES})
# target_link_libraries(gotopose ${catkin_LIBRARIES})
# target_link_libraries(listener ${catkin_LIBRARIES})
# target_link_libraries(repeat ${catkin_LIBRARIES})
# target_link_libraries(gotoposetimeout ${catkin_LIBRARIES})
target_link_libraries(saferandomwalk ${catkin_LIBRARIES})
target_link_libraries(detectdivergence ${catkin_LIBRARIES}$)