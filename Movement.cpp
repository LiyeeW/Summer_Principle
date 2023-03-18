#include "Movement.h"
#include "Robot.h"
#include "Station.h"

#include <cmath>

//用PID算法控制机器人锁定朝向目标工作台
PidControl orient_pid_table[ROBOT_NUM];
//(TODO)首先，在地图初始化阶段，需要调用initPid，设定PID参数
//(TODO)之后，在每次切换目标地点时，需要调用resetPID
//(TODO)在获取到每一帧数据后，需要调用launchPID，获得角速度设定值
//(TODO)PID的参数需要结合判题器做实验，根据经验人为设置


//(TODO)每个机器人的当前目标工作台，每次切换目标地点时更新
int dest_station[ROBOT_NUM];



//计算机器人到工作台的直线路径的目标角度偏向
float cacuPathOrient(int robot_id, int station_id){
    float dy = station_info_table[station_id].y - robot_info_table[robot_id].y;
    float dx = station_info_table[station_id].x - robot_info_table[robot_id].x;
    return (float)atan(dy/dx);
}

//初始化角速度的pid(目前带参数，为方便做实验找PID参数)
float orientInitPid(float kp, float ki, float kd){
    initPidControlTable(ROBOT_NUM, orient_pid_table, kp, ki, kd, ORIENT_LIMIT, -ORIENT_LIMIT);
}

//进行一轮PID控制，返回角速度控制值
float orientLaunchPid(int robot_id){
    //计算在当前位置的目标角度
    float dest = cacuPathOrient(robot_id, dest_station[robot_id]);
    //根据帧信息获取当前实际角度
    float real = robot_info_table[robot_id].orient;
    //获取Pid输出
    float output = launchPidControl(robot_id, orient_pid_table, dest-real);
    return output;
}



