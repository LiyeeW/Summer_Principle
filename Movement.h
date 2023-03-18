#ifndef movement_h
#define movement_h

#include "PidControl.h"
#include "Robot.h"


//可调：PID参数
// const float ORIENT_KP;
// const float ORIENT_KI;
// const float ORIENT_KD;

//可调：直线距离多少米且源材料还未生产好时，开始调整运动阶段
const float APPROACH_DISTANCE = 1.5;

// PI
const float PI = 3.14159f;
// 角速度上下限的绝对值
const float ORIENT_LIMIT = PI;
//线速度的上下限
const float DIRECT_UP_LIMIT = 6;
const float DIRECT_LOW_LIMIT = -2;



//直线距离表
 extern float direct_distance_table[ROBOT_NUM];
/* 用于调整单个机器人的角速度和速度
 * 1.0版本的策略：
 * 1）方向相差在正负15度之内，并且距离相差0.02s * 6m/s * 5 =0.6m ，则调整角速度为0，速度6m全速前进
 * 2）不满足方向，则调整速度为0，角速度为2全速满足方向
 * 3）方向满足，距离相差为0.6m，
*/

//获取机器人的目标工作台
int getRobotDest(int robot_id);

//进行一轮PID控制，返回角速度控制值
float orientLaunchPid(int robot_id);





#endif