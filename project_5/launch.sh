#!/bin/sh
xterm  -e  " gazebo " &
sleep 2
xterm  -e  " source /opt/ros/kinetic/setup.bash; roscore" &
sleep 2
xterm  -e  " rosrun rviz rviz"
