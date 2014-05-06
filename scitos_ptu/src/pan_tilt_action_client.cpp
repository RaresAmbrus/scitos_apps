#include <scitos_ptu/PanTiltAction.h>
#include <scitos_ptu/PanTiltGoal.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<scitos_ptu::PanTiltAction> Client;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pan_tilt_client");
  Client client("ptu_pan_tilt_metric_map", true); // true -> don't need ros::spin()
  client.waitForServer();
  scitos_ptu::PanTiltGoal goal;
  // Fill in goal here
  goal.pan_start= -100;
  goal.pan_end= 100;
  goal.pan_step= 50;
  goal.tilt_start= -30;
  goal.tilt_step= 20;
  goal.tilt_end= 30;
  client.sendGoal(goal);
  client.waitForResult(ros::Duration(5.0));
  if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    printf("Yay! The dishes are now clean");
  printf("Current State: %s\n", client.getState().toString().c_str());
  return 0;
}
