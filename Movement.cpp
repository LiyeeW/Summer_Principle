#include "Movement.h"
#include "Robot.h"
#include "Station.h"

#include <cmath>

//用PID算法控制机器人锁定朝向目标工作台
PidControl orient_pid_table[ROBOT_NUM];
//首先，在地图初始化阶段，需要调用initPid，设定PID参数
//之后，在每次切换目标地点时，需要调用resetPID
//在获取到每一帧数据后，需要调用launchPID，获得角速度设定值
//PID的参数需要结合判题器做实验，根据经验人为设置



//计算机器人到工作台的直线路径的目标角度偏向
float cacuPathOrient(int robot_id, int station_id){
    float dy = station_info_table[station_id].y - robot_info_table[robot_id].y;
    float dx = station_info_table[station_id].x - robot_info_table[robot_id].x;
    return (float)atan(dy/dx);
}


