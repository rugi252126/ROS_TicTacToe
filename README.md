# ROS TicTacToe

## 1. Introduction
ROS_TicTacToe is basically the game TicTacToe that developed over ROS environment.
ROS is an open source that mainly intended for robots. So, it is not ideal to involve ROS in developing the game TicTacToe. And yes, TicTacToe game can be developed even without ROS. 
However, this is not my intention here. The main intention is to develop a low-cost platform for those people who are new to ROS and wanted to play around with ROS before moving to more complex system.
By developing this project, you will learn on how to create your own workspace, ROS package, node, message, topic, launch file and how to integrate them to ROS environment. 
In addition, you will also learn on how to communicate(publish/subscribe) between ARM based development board/computer and micorcontroller.
And finally, at the end you will learn not only ROS but have also a complete understanding regarding the whole process when building a complete project(from Requirement point of view, Hardware, Software and so on).


## 1.1 Folder Structure
ROS_TicTacToe
 - Hardware
 - Requirement
 - Software
 
### 1.1.1 Hardware Folder
This folder contains information like schematic diagram, BOM, datasheet and anything related to hardware.

### 1.1.2 Requirement Folder
This folder contains the System requirements, Communication Matrix and Schedule Table requirements.

### 1.1.3 Software Folder
This folder contains installation script, software design, rosserial library, manually created ROS packages and messages, the TicTacToe game software and anything related to software.


## 2. Environment Setup

## 2.1 Installation

### 2.1.1 ROS Installation
Make sure you have Linux OS. Install the ROS distro depending on your Ubuntu release version.
For Ubuntu 16.04, it is the ROS Kinetic. 
http://wiki.ros.org/kinetic/Installation/Ubuntu

And for Ubuntu 18.04, it is the ROS Melodic.
http://wiki.ros.org/melodic/Installation/Ubuntu


For Ubuntu desktop/latop, install the "Desktop-Full Install". And for ARM based development board, install the "ROS-Base: (Bare Bone)".
For this project, the use of desktop/laptop to run the GUI tools is optional. But, we will also cover here to understand on how to configure the environment.


## 2.2 Network Setup
The network configuration has to be done if it is not yet done before or everytime the ip-address of the machine is changed.

### 2.2.1 ARM based development board or computer/laptop alone
To configure your ROS network, do the following.

- Open the terminal
- Check your current configuration. Type: printenv | grep ROS
- If your ROS_MASTER_URI is not yet assigned to localhost, do the assignment
- Type the following: export ROS_MASTER_URI=http://localhost:11311
- Check your configuration again to see your changes

You have to do this export ROS_MASTER_URI=http://localhost:11311 everytime you open new terminal.
To avoid doing the same thing, you can automate this process by doing the following.

- Open the terminal
- Type: echo "export ROS_MASTER_URI=http://localhost:11311" >> ~/.bashrc

### 2.2.2 ARM based development board and computer/laptop
To configure your ROS network, do the following.

- Open the terminal
- Check the ip-address of your ARM board and computer/laptop
- Type: ifconfig
- Take note of the ip-addresses

Computer/Laptop:
- Type the following in the terminal.
- export ROS_MASTER_URI=http://<ARM board ip-address>:11311
- export ROS_HOSTNAME=<computer/laptop ip-address>

ARM board:
- Type the following in the terminal.
- export ROS_MASTER_URI=http://<ARM board ip-address>:11311
- export ROS_HOSTNAME=<ARM board ip-address>


You can automate this process to avoid doing the same thing everytime you open new terminal.

Computer/Laptop:
- Type the following in the terminal.
- echo "export ROS_MASTER_URI=http://<ARM board ip-address>:11311" >> ~/.bashrc
- echo "export ROS_HOSTNAME=<computer/laptop ip-address>" >> ~/.bashrc

ARM board:
- Type the following in the terminal.
- echo "export ROS_MASTER_URI=http://<ARM board ip-address>:11311" >> ~/.bashrc
- echo "export ROS_HOSTNAME=<ARM board ip-address>" >> ~/.bashrc


## 2.3 uDev Rules
Since the arduino microcontroller is connected via USB of the ARM board computer, we have to defined and enabled the USB port used by arduino microcontroller.
It can be done in different ways. Below I'll show you two methods.
1. First method, it needs to be done manually everytime the ARM board computer is power-up.
- Open the terminal
- Plug-in your microcontroller USB to ARM board computer USB port
- To check the current permision of the USB port, type: ls -la /dev/ttyUSB0
- To enable the read and write permission, type: sudo chmod a+rw /dev/ttyUSB0

2. Second method is to do it permanently by applying new rules. Permanently means you don't have to do it everytime the ARM board computer is power-up. 
You just have to do it once. To do it, please do the following
- Open the terminal
- Then type: cd ~/ros_tictactoe_ws/src/udev_rules
- Run the ROS package to run the script, type: rosrun udev_rules make_udev.py
- Plug-in your microcontroller USB to ARM board computer USB port
- Once you see the message "New udev rules is successfully created.", press "CTRL+C" and follow the instruction given in the terminal.
- Copy the new rules, type: sudo cp 25-tictactoe.rules /etc/udev/rules.d/25-tictactoe.rules
- Reboot your computer

To check the detailed information of the device, you can do the following.
- udevadm info -a -n /dev/ttyUSBx OR udevadm info -a -n /dev/ttyACMx
- x can be 0, 1, etc..

For Ubuntu 16.04, method number 2 can be done without any problem. However, for Ubuntu 18.04 the script won't work because the gudev in python is not supported. 
For Ubuntu 18.04, if you are planning to create new udev rules manually, you can use below link as reference.
- Detailed explanation about the udev rules and how to create one
- http://www.reactivated.net/writing_udev_rules.html


## 2.4 Project Installation
ARM board computer:
To install the needed tools and packages, create and configure project workspace, the installation package has to be started.
Please follow below procedure.

- Open the terminal
- cd
- git clone https://github.com/rugi252126/ROS_TicTacToe.git
- cd ~/ROS_TicTacToe/Software/Tools/Installer
- ./setup
- If installation is successful, at the end of the installation process you will see the message "Installation is done."
- Go to your home directory and you can check the newly created project workspace named "ros_tictactoe_ws"
- Try to build the project workspace
- cd ~/ros_tictactoe_ws/src
- catkin_make
- The project workspace should be able to build without any problem


## 3. Hardware Setup

## 3.1 Linux Computer/Laptop
I tested the system using different machines.
Using Vorke mini computer where Ubuntu 16.04 is installed. And also using Raspberry Pi 3B+ where Ubuntu 18.04 is installed.
From the design point of view, the game should work fine if you use other development boards.

## 3.2 Microcontroller
The arduino mega2560 microcontroller is used for the following reasons.
- It has enough RAM/ROM to fit the ROS environment and the needed software implementation.
- It is not difficult to establish communication between ARM board/computer/laptop and arduino because rosserial_arduino library is available.
- It has enough IO pins to control everything as specify in the Requirements.

## 3.3 Batteries
- Powerbank
- LiPo 2S battery or equivalent

For more information regarding the voltage and current rating, please check the Requirements.

## 3.4 Schematic and Part list
Please see the BOM for the complete list of parts and schematic diagram for the wiring connnection.


## 4. Software
The software implementation is distributed between ARM board computer and microcontroller board.
For more information, please check the Software Architecture document.

## 4.1 Platformio IO
It is used to build and load the software into arduino microcontroller.


## 5. Testing
If everything is fine, it is time to load the software to the Base Control Unit and start the testing.

## 5.1 Loading Firmware
To load the software into microcontroller, please do the following.
- Open the terminal
- cd ~/ros_tictactoe_ws/src/tictactoe/arduinomega
- platformio run --target upload
- You should able to see message that flashing is successfully done.

## 5.2 Start Communication
After firmware was successfully loaded, then start the communication by doing the following.
- Open the terminal
- cd ~/ros_tictactoe_ws/
- roslaunch tictactoe tictactoe.launch

## 6. Photo and Video

## 6.1. Actual Photo
![Blue LED](https://github.com/rugi252126/ROS_TicTacToe/blob/master/Hardware/Prototype/SAM_9105.JPG?raw=true)

![Red LED](https://github.com/rugi252126/ROS_TicTacToe/blob/master/Hardware/Prototype/SAM_9107.JPG?raw=true)

## 6.2. Video link
- https://drive.google.com/file/d/1upj7tLbBJ0uBpl8KDFbGav7eW70T2_4d/view?usp=sharing







