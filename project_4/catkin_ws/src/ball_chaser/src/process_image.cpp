#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <unistd.h>
#include <vector>


// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv)){
          ROS_INFO_STREAM("Failed to drive the robot...");
        }

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    std::vector<int> whitepixels;
    int white_pixel = 255;
    float x_speed = 0.0;
    float z_speed = 0.0;
    // ROS_INFO("height = %d, step = %d", img.height, img.step);
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    for (int i = 0; i < img.height; i++){ //height is 800
        for (int j = 0; j < img.step; j+=3){ // step is 2400

            if (img.data[img.step*i + j] == white_pixel &&
                img.data[img.step*i + j + 1] == white_pixel &&
                img.data[img.step*i + j + 2] == white_pixel){

                // storing the location of white pixels:

                whitepixels.push_back(j);
            }
        }
    }

    // Average the location of white pixels in the image:

    int avrage_whitepixels = std::accumulate(whitepixels.begin(), whitepixels.end(), 0) / whitepixels.size();

    if (9 * avrage_whitepixels < 4 * img.step) {
            x_speed = 0.0;
            z_speed = 1;
        }else if (9 * avrage_whitepixels > 5 * img.step) {
            x_speed = 0.;
            z_speed = -1;
        }else{
            x_speed = 1;
            z_speed = 0.;
                }


    drive_robot(x_speed, z_speed);
}




// Yes, correct. It is a single array, so to access ith row jth element it would be something like image.data[image.step*i+j*3]
//


int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
