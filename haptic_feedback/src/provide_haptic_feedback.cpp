#include <sound_play/sound_play.h>
#include <unistd.h>
#include <baxter_core_msgs/EndEffectorState.h>

double g_latest_gripper_force;

void sleepok(int t, ros::NodeHandle &nh){
	if(nh.ok()){
		sleep(t);
	}
}

//TODO: I may need an include statement to support the message type below

void gripperStateCallback(const baxter_core_msgs::EndEffectorState& new_state){
	g_latest_gripper_force = new_state.force;
}

int main(int argc, char** argv){
	ros::init(argc, argv, "sound_play_test");
	ros::NodeHandle nh;

	ros::Subscriber gripper_state_subscriber = nh.subscribe("/robot/end_effector/right_gripper/state",1,gripperStateCallback);

	sound_play::SoundClient sc;
	
	sleepok(1,nh);//I think I can remove this one
	
	while(nh.ok()){
	
		if(g_latest_gripper_force < 20.0){
			ROS_INFO("Playing quiet sound: %f", g_latest_gripper_force);
			sc.playWave("/home/kevinbradner/ros_ws/src/eecs_589_baxter_reactive_teleop/haptic_feedback/wav_files/250hz_tenth_vol2.wav");
		}else{
			ROS_INFO("Playing loud sound");
			sc.playWave("/home/kevinbradner/ros_ws/src/eecs_589_baxter_reactive_teleop/haptic_feedback/wav_files/250hz_tenth_vol1.wav");
		}

		ros::Duration(0.1).sleep();
		ros::spinOnce();
	}
	
	return 0;
}
