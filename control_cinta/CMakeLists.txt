cmake_minimum_required(VERSION 2.8.3)
project(control_cinta)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  roslib
  std_msgs
  rosserial
)

catkin_package()


include_directories(

  ${catkin_INCLUDE_DIRS}
)

add_executable(control_cinta src/control.cpp) #nodo/cpp

target_link_libraries(control_cinta ${catkin_LIBRARIES})

