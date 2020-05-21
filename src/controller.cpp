#include <ros/ros.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include<geometry_msgs/Twist.h>
#include<assignment_1/data.h>

//#include<std_msgs/Vectors.h>
using namespace std;
float x,y,z;
float KP=0.8,KI=0.6,KD=2;
std::string data;
geometry_msgs::Twist target_velocity;
nav_msgs::Odometry target_pos;
float iteration_time=0.5;
int i=0;
float pid(float desired_value,float actual_value){
    float error_prior=0;
    float integral_prior=0;
    float error = desired_value - actual_value;
    float integral = integral_prior + error * iteration_time;
    float derivative = (error - error_prior) / iteration_time;
    float output = KP*error + KI*integral + KD*derivative ;
    error_prior = error;
    integral_prior = integral;
   }
//callback function which suscribes the target positions from the topic named-"input_topic"
void input_callback(assignment_1::data odom){

 (target_pos.pose.pose.position.x) = odom.coordinates.data[i];
 (target_pos.pose.pose.position.y) = odom.coordinates.data[i+1];
 
return;
}


//callback function which suscribes the current coordinates of the bot(updates x,y,z every moment)
void subCallback(nav_msgs::Odometry odom){
 x = odom.pose.pose.position.x;
 y = odom.pose.pose.position.y;
 z = odom.pose.pose.position.z;
 return;}


int main(int argc, char** argv){
ros::init(argc, argv, "feedback_controller");
ros::NodeHandle nh;
std::string topic_setpoint,topic_current,topic_velocity;
double rate;
nh.getParam("topic_setpoint",topic_setpoint);
nh.getParam("topic_current_position",topic_current);
nh.getParam("topic_target_velocity",topic_velocity);
nh.getParam("rate",rate);
ros::Subscriber sub_target = nh.subscribe(topic_setpoint,10,input_callback);                         //subscribe target setpoints from "input_topic"  
ros::Subscriber sub = nh.subscribe(topic_current,10, subCallback);                           //subscribe the current position from "odometry/filtered" topic
ros::Publisher pub = nh.advertise<geometry_msgs::Twist>(topic_velocity,10);  //publishes the velocity data on the topiv "/husky_velocity_controller/cmd_vel"
ros::Rate loopRate(rate);


while(ros::ok()){
ros::spinOnce();


target_velocity.linear.x= pid(target_pos.pose.pose.position.x,x);
target_velocity.linear.y=pid(target_pos.pose.pose.position.y,y);

if((abs(target_velocity.linear.x-x)<0.02)&&(abs(target_velocity.linear.y-y)<0.02)) i+=2;//as bot reaches one of the required position,increment i and start looking for the next target position
pub.publish(target_velocity);
loopRate.sleep();
}
return 0;
}
