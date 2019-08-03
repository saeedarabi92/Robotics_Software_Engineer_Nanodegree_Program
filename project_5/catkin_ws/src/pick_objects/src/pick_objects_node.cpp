#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/UInt8.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle n;

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //set up publisher to broadcast if robot is at pick up location
  ros::Publisher location_pub = n.advertise<std_msgs::UInt8>("/destination_reached", 1);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal_1;
  move_base_msgs::MoveBaseGoal goal_2;

  // set up the frame parameters for goal_1
  goal_1.target_pose.header.frame_id = "map";
  goal_1.target_pose.header.stamp = ros::Time::now();

  // set up the frame parameters for goal_2
  goal_2.target_pose.header.frame_id = "map";
  goal_2.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal_1.target_pose.pose.position.x = -5.0;
  goal_1.target_pose.pose.position.y = 4.0;
  goal_1.target_pose.pose.orientation.w = 1.0;

 // Define a position and orientation for the robot to reach
  goal_2.target_pose.pose.position.x = 1.0;
  goal_2.target_pose.pose.position.y = -5.0;
  goal_2.target_pose.pose.orientation.w = -1.0;



   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending the pickup zone");
  ac.sendGoal(goal_1);

  std_msgs::UInt8 msg;
  msg.data = 0;
  location_pub.publish(msg);
  ROS_INFO("location_pub published  %d ", msg.data);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {ROS_INFO("Hooray, the robot reached the pickup zone");
    //Notify the add_markers_node that the first goal has been reached
    std_msgs::UInt8 msg;
    msg.data = 1;
    location_pub.publish(msg);
    ROS_INFO("location_pub published %d ", msg.data);
    // wait 5 seconds before going to the next goal
    sleep(5);
  }else{
    ROS_INFO("The robot failed to reach the pickup zone");
    }

  // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending the drop off zone");
  ac.sendGoal(goal_2);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  { ROS_INFO("Hooray, the robot reached the drop off zone");
   // wait 5 seconds before going to the next goal
   //Notify the add_markers_node that the second goal has been reached
   std_msgs::UInt8 msg;
   msg.data = 1;
   location_pub.publish(msg);
   sleep(5);
  }else{
   ROS_INFO("The base failed to reach the robot reached the drop off zone");

  return 0;}


}
