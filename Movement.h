#ifndef movement_h
#define movement_h

#include "PidControl.h"
#include "Robot.h"

using namespace std;

//可调：PID参数
// const float ORIENT_KP;
// const float ORIENT_KI;
// const float ORIENT_KD;

//可调：直线距离多少算临近，单位m
const float APPROACH_DISTANCE = 1.5;
//可调：角度偏差的绝对值在多少以内算方向对准
// const float LOCK_ORIENT = 0.1;
//可调：角速度的绝对值在多少以内算近乎为零
const float LOCK_OMEGA = 0.1;
//可调：角速度保持近乎为零要连续多少帧才算方向稳定
const int LOCK_FRAME = 10;


const float PI = 3.14159f;
// 角速度上下限的绝对值
const float ORIENT_LIMIT = PI;
//线速度的上下限
const float DIRECT_UP_LIMIT = 6;
const float DIRECT_LOW_LIMIT = -2;
// 每一帧的时间间隔，单位s
const float FRAME_SECOND = 0.02;

//机器人关于运动的重要信息
typedef struct{
    int stage;           //运动阶段，1是直线运动，2是低速定向，3是高速定向
    int destStation;    //目的工作台
    float destOrient;   //到目的地的直线朝向
    float destDistance; //到目的地的直线距离
    bool destWait;      //目的地是否需要等待
    PidControl pidOrient;//角度的PID控制信息
    PidControl pidDistance;//距离的PID控制信息
    //TODO
    bool reverse;       //在倒退，只可能在阶段一出现倒退
    bool approached;    //已临近目的地
    bool locked;        //方向已稳定锁定
    int maybeFrame;     //预计到达时的帧号
    //TODO 避障
} RobotMove;
 
 //机器人运动信息表
extern RobotMove robot_move_table[ROBOT_NUM];

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