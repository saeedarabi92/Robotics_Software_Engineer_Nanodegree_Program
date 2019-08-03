#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/UInt8.h>



//initialize global variable keeping track of the current search phase
uint8_t counter = 0;
void deliverycallback(const std_msgs::UInt8::ConstPtr& msg){

    ROS_INFO("Message recieved!");
    counter += msg->data;
    return;
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(5);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber location_sub = n.subscribe("/destination_reached", 1, deliverycallback);
  bool done = false;
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    //Do this every cycle to ensure the subscriber receives the message
    ros::spinOnce();

    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // ROS_INFO("The counter is %d", counter);
    switch (counter)
    {

        case 0: //Adding object for pickup

            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::ADD;
            ROS_INFO_ONCE("Adding object for pickup");
            // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
            marker.pose.position.x = -5.0;
            marker.pose.position.y = 4.0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = -5.0;
            marker.pose.orientation.y = 4.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            break;

        case 1: //deleting picked up object

            ROS_INFO_ONCE("Deleting pickedup object");
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::DELETE;
            counter +=1;
            break;

        case 2: //deleting picked up object

            ROS_INFO_ONCE("Holding the object");
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::DELETE;
            break;

        case 3:
            marker.action = visualization_msgs::Marker::ADD;
            ROS_INFO_ONCE("Adding object at delivery zone");
            // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
            marker.pose.position.x = 1.0;
            marker.pose.position.y = -5.0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 1.0;
            marker.pose.orientation.y = -5.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            done = true;
            break;
        }
    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();


    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);

    if (done){
        return 0;
    }
    r.sleep();
  }
  return 0;
}
