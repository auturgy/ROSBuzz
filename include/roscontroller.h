#pragma once
#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"
#include "mavros_msgs/GlobalPositionTarget.h"
#include "mavros_msgs/CommandCode.h"
#include "mavros_msgs/CommandInt.h"
#include "mavros_msgs/ExtendedState.h"
#include "mavros_msgs/State.h"
#include "mavros_msgs/BatteryStatus.h"
#include "mavros_msgs/Mavlink.h"
#include "sensor_msgs/NavSatStatus.h"
#include <sstream>
#include <buzz/buzzasm.h>
#include "buzzuav_closures.h"
#include "buzz_utility.h"
#include "uav_utility.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ostream>
#include <map>

using namespace std;

namespace rosbzz_node{
	
class roscontroller{

public:
	roscontroller(ros::NodeHandle n_c);
	~roscontroller();
	//void RosControllerInit();
	void RosControllerRun();
	
private:
        
 	double cur_pos[3];
	uint64_t payload;
	std::map< int,  buzz_utility::Pos_struct> neighbours_pos_map;
	int timer_step=0;
	int robot_id=0;
        int oldcmdID=0;
	int rc_cmd;
	int bzz_old_cmd=0;
	std::string bzzfile_name, fcclient_name, rcservice_name,bcfname,dbgfname,out_payload,in_payload; //, rcclient;
	bool rcclient;
	ros::ServiceClient mav_client;
	ros::Publisher payload_pub;
	ros::ServiceServer service;
	ros::Subscriber current_position_sub;
	ros::Subscriber battery_sub;
	ros::Subscriber payload_sub;
	ros::Subscriber flight_status_sub;
	/*Commands for flight controller*/
  	mavros_msgs::CommandInt cmd_srv;	
  	

	void Initialize_pub_sub(ros::NodeHandle n_c);

	/*Obtain data from ros parameter server*/	
	void Rosparameters_get(ros::NodeHandle n_c);

	/*compiles buzz script from the specified .bzz file*/
	void Compile_bzz();

	/*Prepare messages and publish*/
	void prepare_msg_and_publish();

	  	
	/*Refresh neighbours Position for every ten step*/
	void maintain_pos(int tim_step);

	/*Maintain neighbours position*/
	void neighbours_pos_maintain(int id, buzz_utility::Pos_struct pos_arr );

        /*Set the current position of the robot callback*/
	void set_cur_pos(double latitude,
			 double longitude,
			 double altitude);
	/*convert from catresian to spherical coordinate system callback */
	double* cvt_spherical_coordinates(double neighbours_pos_payload []);
	
	/*battery status callback*/ 
	void battery(const mavros_msgs::BatteryStatus::ConstPtr& msg);
	
	/*flight status callback*/
	void flight_status_update(const mavros_msgs::ExtendedState::ConstPtr& msg);
	
	/*current position callback*/
	void current_pos(const sensor_msgs::NavSatFix::ConstPtr& msg);

	/*payload callback callback*/
	void payload_obt(const mavros_msgs::Mavlink::ConstPtr& msg);

	/* RC commands service */
	bool rc_callback(mavros_msgs::CommandInt::Request  &req,
		         mavros_msgs::CommandInt::Response &res);



};

}