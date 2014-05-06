cmake_minimum_required(VERSION 2.8.3)
project(scitos_ptu)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS rospy sensor_msgs genmsg actionlib_msgs actionlib)


add_action_files(DIRECTORY action FILES PanTilt.action)
generate_messages(DEPENDENCIES actionlib_msgs sensor_msgs)

catkin_package(
)

add_executable(pan_tilt_client src/pan_tilt_action_client.cpp)

 target_link_libraries(pan_tilt_client
   ${catkin_LIBRARIES}
 )
