#include "t3_cpu.h"

int main(int argc, char** argv)
{
    // ROS init node t3_cpu_node
    ros::init(argc, argv, "t3_cpu_node");
    T3CpuNode t3_cpu_n;
    ros::spin();
    return 0;
}
