#include <ros/ros.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include<assignment_1/data.h>
//#include<std_msgs/Vectors.h>
using namespace std;
int main(int argc, char** argv){
ros::init(argc, argv, "setpos_pub");
ros::NodeHandle nh;
std::string topic; double rate;int n;
nh.getParam("input_topic", topic);
nh.getParam("iterations",n);
ros::Publisher pub = nh.advertise<assignment_1::data>(topic,10);
ros::Rate loopRate(rate);
//vector<_Float32> point;
assignment_1::data point;

while(ros::ok()){
    //following a random trajectory(y=2*x*x)
    for(int i=0;i<n;i+=2){
        float j = 0.1*i;
    // odom[i].pose.pose.position.x = j;
    // odom[i].pose.pose.position.y = 2*j*j;
    // odom[i].pose.pose.position.z = 0.0;
    
     point.coordinates.data[i]=j;
     point.coordinates.data[i+1]=2*j*j;
    
    }
pub.publish(point);
loopRate.sleep();

return 0;}

}
