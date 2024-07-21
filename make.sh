#!/bin/bash
workspace=$(pwd)

source ~/.bashrc

gnome-terminal -t "ARX7_L" -x  bash -c "cd ${workspace}/ARX7_L; rm -rf build devel .catkin_workspace src/CMakeLists.txt; catkin_make clean ;catkin_make; exec bash;"

gnome-terminal -t "ARX7_R" -x  bash -c "cd ${workspace}/ARX7_R; rm -rf build devel .catkin_workspace src/CMakeLists.txt; catkin_make clean ;catkin_make; exec bash; "



