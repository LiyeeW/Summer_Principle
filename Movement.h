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
const float APPROACH_DISTANCE = 3.5;
//可调：角度偏差的绝对值在多少以内可以加速
const float LOCK_ORIENT = 0.8;
//可调：角速度的绝对值在多少以内算近乎为零
const float LOCK_OMEGA = 0.1;
//可调：角速度保持近乎为零要连续多少帧才算方向稳定
const int LOCK_FRAME = 1;
//可调：在阶段一，角度差大于多少算偏离
const float ORIENT_LOST = 0.05;
//可调：浮点精度的裕度
const float FLOAT_MARGIN = 0.05;


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
    int destStation;    //目的工作台，每次切换目标地点时更新
    float destOrient;   //到目的地的直线朝向
    int lastSwingFrame; //上一次方向和角速度不满足的帧号
    bool destPass;      //是否已越过目的地
    float destDistance; //到目的地的直线距离
    bool destWait;      //目的地是否需要等待
    PidControl pidOrient;//角度的PID控制信息
    PidControl pidDistance;//距离的PID控制信息
    float nextOmega;    //下一步的角速度
    float nextSpeed;    //下一步的线速度
    //TODO
    // bool reverse;       //在倒退，只可能在阶段一出现倒退
    // int maybeArriveFrame;     //预计到达时的帧号
    //TODO 避障
} RobotMove;
 
 //机器人运动信息表
extern RobotMove robot_move_table[ROBOT_NUM];

//获取机器人的目标工作台
int getRobotDest(int robot_id);

//进行一轮PID控制，返回角速度控制值
float orientLaunchPid(int robot_id);

//获取机器人下一步的角速度
float getRobotNextOmega(int robot_id);

//获取机器人下一步的线速度
float getRobotNextSpeed(int robot_id);

//获取机器人是否临近目的地，此时距离不可能为负
bool getRobotApproached(int robot_id);

//获取目的地是否需要等待
bool getRobotDestWait(int robot_id);

//运动系统必要的全局初始化工作
void initMoveGlobal();

//每帧更新运动记录，包括直线角度与距离、是否需要等待、运动阶段等
void updateMovePerFrame();

//根据运动阶段和PID算法得到下一步的角速度和线速度
void moveByStage(int robot_id);

//在某机器人朝新的目的地出发前，对运动系统的重置工作
void resetMoveBeforeDepart(int robot_id);

#endif