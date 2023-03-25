#ifndef pid_control_h
#define pid_control_h

#include "Robot.h"

//每个机器人有三个PID，依次是角度、距离和线速度
const int PID_NUM = 3;

//积分系数不能超过输出上下限的比例
const float I_MAX_RATE = 0.8;

//对某个变量的PID控制的所需信息
 typedef struct {
    float offset;  //偏移值，即目标值-实际值
    float sum_offset;   //累计的偏移值
    float dif_offset;   //本轮与上一轮的偏移值的差
    float kp;        //比例系数p
    float ki;        //积分系数i
    float kd;        //微分系数d
    float up_limit;  //输出的上限
    float low_limit;    //输出的下限
 } PidControl;


//初始化一组PID控制
void initPidControlTable(PidControl* pc, float kp, float ki, float kd, float up_limit, float low_limit);

//重置一个PID控制
void resetPidControl(PidControl* pc);

//启动一个PID控制的一轮迭代，返回PID控制的输出值
float launchPidControl(PidControl* pc, float offset);

//执行-运动-PID，在游戏开始时，全局的初始化工作
void initPidGameStart();

//执行-冲突-PID，在运动阶段开始时，单个机器人的重置工作
void resetPidStageStart(int robot_id, int pid_id);

//执行-冲突-PID，在运动阶段执行时，单个机器人的运转工作
float launchPidStageExecute(int robot_id, int pid_id, float offset);


#endif 