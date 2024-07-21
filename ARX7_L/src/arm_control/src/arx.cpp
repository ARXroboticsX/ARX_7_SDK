#include <ros/ros.h>
#include <cmath>
#include <iostream>
#include <std_msgs/Float32MultiArray.h>
#include "utility.h"
#include "Hardware/can.h"
#include "Hardware/motor.h"
#include "Hardware/teleop.h"
#include "App/arm_control.h"
#include "App/keyboard.h"
#include "App/play.h"
#include "App/solve.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <atomic>
#include <signal.h>
#include "arm_control/PosCmd.h"
#include "arm_control/JointControl.h"
#include "arm_control/ChassisCtrl.h"

int CONTROL_MODE=0; //0left  1right
command cmd;

bool app_stopped = false;
void sigint_handler(int sig);
void safe_stop(can CAN_Handlej);
bool ros_cmd = false;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "arx7_l"); 
    ros::NodeHandle node;
    Teleop_Use()->teleop_init(node);

    arx_arm ARX_ARM((int) CONTROL_MODE);

    ros::Subscriber sub_cmd = node.subscribe<arm_control::PosCmd>("/ARX_VR_L", 10, 
                                [&ARX_ARM](const arm_control::PosCmd::ConstPtr& msg) {
                                    ARX_ARM.arx5_cmd.x_t          = msg->x;
                                    ARX_ARM.arx5_cmd.y_t          = msg->y;
                                    ARX_ARM.arx5_cmd.z_t          = msg->z;
                                    ARX_ARM.arx5_cmd.waist_roll = msg->roll;
                                    ARX_ARM.arx5_cmd.waist_pitch  = msg->pitch;
                                    ARX_ARM.arx5_cmd.waist_yaw_t    = msg->yaw;
                                    ARX_ARM.arx5_cmd.gripper      = msg-> gripper;
                                });

    arx5_keyboard ARX_KEYBOARD;

    ros::Rate loop_rate(200);
    can CAN_Handlej;

    std::thread keyThread(&arx5_keyboard::detectKeyPress, &ARX_KEYBOARD);
    sleep(1);

    while(ros::ok())
    { 

        char key = ARX_KEYBOARD.keyPress.load();
        ARX_ARM.getKey(key);

        ARX_ARM.get_joint();
        if(ros_cmd)
        {ARX_ARM.limit_cmd();}
        else
        {ARX_ARM.get_cmd();}
        
        ARX_ARM.update_real(cmd);
    
        ros::spinOnce();
        loop_rate.sleep();
        CAN_Handlej.arx_1();
        
    }
    CAN_Handlej.arx_2();
    return 0;
}