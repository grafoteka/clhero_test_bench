cmake_minimum_required(VERSION 2.8.3)
project(escritor_txt)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  roslib
  std_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)


add_executable(escritor_txt src/escritor_txt.cpp) #nodo/cpp

target_link_libraries(escritor_txt ${catkin_LIBRARIES})
