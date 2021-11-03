cmake_minimum_required(VERSION 2.8.3)
project(tictactoe)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  rospy
  roscpp
  tictactoe_msgs
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES tictactoe
  CATKIN_DEPENDS rospy roscpp tictactoe_msgs std_msgs
  DEPENDS system_lib
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(Action src/Action.cpp)
add_library(Player src/Player.cpp)
add_library(Board src/Board.cpp)
add_library(t3_cpu src/t3_cpu.cpp)
add_executable(t3_cpu_node src/t3_cpu_node.cpp) ## When adding new library, first, comment out this line then catkin_make. After that, enable again and catkin_make
target_link_libraries(t3_cpu_node t3_cpu Action Player Board ${catkin_LIBRARIES})


