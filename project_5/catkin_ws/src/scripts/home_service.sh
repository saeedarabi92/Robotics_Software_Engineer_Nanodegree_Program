#!/bin/sh

catkin_dir=/home/saeed/Desktop/github/Robotics_Software_Engineer_Nanodegree_Program/project_5/catkin_ws

xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch " &
sleep 5
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch" &
sleep 2
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 10
xterm  -e  " source $catkin_dir/devel/setup.bash; rosrun add_markers add_markers_node" &
sleep 3
xterm  -e  " source $catkin_dir/devel/setup.bash; rosrun pick_objects pick_objects_node"
