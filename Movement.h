#ifndef movement_h
#define movement_h

#include "PidControl.h"
#include "Robot.h"

using namespace std;

//调转角度的预估帧数
const int ORIENT_FRAME_COST = 25;
//其他环节的预估花费帧数
const int OTHER_FRAME_COST = 15;

//可调：PID参数
// const float ORIENT_KP;
// const float ORIENT_KI;
// const float ORIENT_KD;

//可调：直线距离多少算临近，单位m
const float APPROACH_DISTANCE = 3.5;
//可调：角度偏差的绝对值在多少以内可以加速
const float LOCK_ORIENT = 0.2;
//可调：角速度的绝对值在多少以内算近乎为零
const float LOCK_OMEGA = 0.1;
//可调：角速度保持近乎为零要连续多少帧才算方向稳定
const int LOCK_FRAME = 1;
//可调：在阶段一，角度差大于多少算偏离
const float ORIENT_LOST = 0.05;
//可调：浮点精度的裕度
const float FLOAT_MARGIN = 0.05;
//横坐标差小于多少时，角度算垂直
const float TAN_X_MIN = 0.001;


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


//功能函数：返回s1到s2的估计总花费帧数
int getStationsFrameCost(int s1, int s2);

//功能函数：返回s1到s2的估计总花费帧数，并且机器人是从s0到的s1
int getStationsFrameCost(int s1, int s2, int s0);


//获取机器人是否临近目的地，此时距离不可能为负
bool getRobotApproached(int robot_id);

//获取目的地是否需要等待
bool getRobotDestWait(int robot_id);

//修改机器人运动阶段
void setRobotMoveStage(int robot_id, int stage);

//获取机器人运动阶段
int getRobotMoveStage(int robot_id);

//修改机器人的目标工作台
void setRobotDest(int robot_id, int station_id);

//获取机器人的目标工作台
int getRobotDest(int robot_id);

//重置某机器人的目标工作台，此时task_status只可能是0或1
void resetRobotDest(int robot_id);

//获取机器人的目的工作台的x坐标
float getRobotDestAxisX(int robot_id);

//获取机器人的目的工作台的y坐标
float getRobotDestAxisY(int robot_id);

//修改机器人到目标工作台的直线朝向
void setRobotDestOrient(int robot_id, float orient);

//获取机器人到目标工作台的直线朝向
float getRobotDestOrient(int robot_id);

//获取机器人到目标工作台的直线朝向-机器人实际朝向的差量
float getRobotDestOrientOffset(int robot_id);

//判断机器人的方向和角速度是否满足要求
bool getRobotSwing(int robot_id);

//修改上一次方向和角速度不满足的帧号
void setRobotLastSwingFrame(int robot_id, int frame);

//获取上一次方向和角速度不满足的帧号
int getRobotLastSwingFrame(int robot_id);

//如果当前帧仍在摇摆，记录下来
void updateRobotLastSwingFrame(int robot_id);

//获取机器人是否大体调向成功，能够加速
bool getRobotOrientLocked(int robot_id);

//a，b均为正的浮点数，比较在[-PI,PI]范围内，a是否近似小于b，用于阶段一的近距离差角判断
bool radianLessThan(float a, float b);

//获取机器人是否精确调向成功，能够直线移动
bool getRobotOrientLockedAccurate(int robot_id);

//修改机器人是否相比出发时已越过目的地
void setRobotDestPass(int robot_id, bool pass);

//获取机器人是否相比出发时已越过目的地
bool getRobotDestPass(int robot_id);

//反转机器人是否相比出发时已越过目的地
void flipRobotDestPass(int robot_id);

//修改机器人到目标工作台的直线距离
void setRobotDestDistance(int robot_id, float distance);

//获取机器人到目标工作台的直线距离，若已越过，则返回负值
float getRobotDestDistance(int robot_id);

//修改目的地是否需要等待
void setRobotDestWait(int robot_id, bool wait);

//获取目的地是否需要等待
bool getRobotDestWait(int robot_id);

//获取机器人是否临近目的地，此时距离不可能为负
bool getRobotApproached(int robot_id);

//修改机器人下一步的角速度
void setRobotNextOmega(int robot_id, float omega);

//获取机器人下一步的角速度
float getRobotNextOmega(int robot_id);

//修改机器人下一步的线速度
void setRobotNextSpeed(int robot_id, float speed);

//获取机器人下一步的线速度
float getRobotNextSpeed(int robot_id);

//在阶段三时，通过预估匀速到达需要等待的地点
float getAverageSpeed(int robot_id);

//(每一帧)更新某机器人的到目的地的直线方向、锁定、距离、靠近、越过等信息
void updateRobotDestDirect(int robot_id);

//(每一帧)更新机器人是否需要在临近目的地时降速，等待生产
void updateRobotDestWait(int robot_id);

//前提：robot刚拿到货物，目前所在是拿货工作台
int getLimitWaitFrameOfDest(int robot_id,int limit);

//前提：station_id是已经分配的某个任务的source/dest，这样才能保证一定时间内是可以 拿到/卖出货
int getItemFrameOfStaion(int station_id,int item,int limit);

#endif