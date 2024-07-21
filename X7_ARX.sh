#!/bin/bash
source ~/.bashrc

workspace=$(pwd)


gnome-terminal -t "ARX7_L" -x sudo bash -c "cd ${workspace}/ARX7_L; source devel/setup.bash && roslaunch arm_control X7.launch; exec bash;"
sleep 1
gnome-terminal -t "ARX7_R" -x sudo bash -c "cd ${workspace}/ARX7_R; source devel/setup.bash && roslaunch arm_control X7.launch; exec bash;"
