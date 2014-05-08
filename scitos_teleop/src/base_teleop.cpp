#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <ros/console.h>
#include <sensor_msgs/JointState.h>
#include <math.h>

ros::Publisher pub_ptu_cmd;
ros::Publisher pub_cmd_vel;
double l_scale_, a_scale_;
sensor_msgs::JointState ptuState;  

geometry_msgs::Twist t;

void controlCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
  t.linear.x = 0.9*t.linear.x + 0.1*l_scale_ * msg->axes[1];
  t.angular.z = 0.5*t.angular.z + 0.5*a_scale_ * msg->axes[0];
	if(msg->header.frame_id == "stop") {
		t.linear.x = 0.0;
		t.angular.z = 0.0;
	}
	pub_cmd_vel.publish(t);

   sensor_msgs::JointState newptuState = ptuState;
   newptuState.position[0] = ptuState.position[0];
   newptuState.position[1] = ptuState.position[1];

   static int count = 0;

   if (count < 5)
   {
     count++;
     return;
   }

   count=0;
   if (msg->axes[2] < 0) 
   { 
	newptuState.position[0] = ptuState.position[0] - M_PI/36.0;
   } else if (msg->axes[2]>0)
   {
	newptuState.position[0] = ptuState.position[0] + M_PI/36.0;
   }

   if (msg->axes[3] < 0) 
   { 
	newptuState.position[1] = ptuState.position[1] - M_PI/36.0;
   } else if (msg->axes[3]>0)
   {
	newptuState.position[1] = ptuState.position[1] + M_PI/36.0;
   }

   pub_ptu_cmd.publish(newptuState);

}


void ptuCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
	ptuState = *msg;
	std::cout<<"Ptu state "<<ptuState.position[0]<<"  "<<ptuState.position[1]<<std::endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "teleop_base");

  ros::NodeHandle n("teleop_base");
  n.param("scale_angular", a_scale_, 0.8);
  n.param("scale_linear", l_scale_, 0.8);

  ros::Subscriber sub = n.subscribe("/teleop_joystick/joy", 1000, controlCallback);
  ros::Subscriber sub2 = n.subscribe("/ptu/state", 1000, ptuCallback);
  pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  pub_ptu_cmd = n.advertise<sensor_msgs::JointState>("/ptu/cmd", 1000);
  
  ros::spin();

  return 0;
}
