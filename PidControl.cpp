#include "Score.h"
#include "PidControl.h"
#include <cmath>
#include <iostream>

using namespace std;

//初始化一组PID控制
void initPidControlTable(PidControl* pc, float kp, float ki, float kd, float up_limit, float low_limit){
    pc->kp = kp;
    pc->ki = ki;
    pc->kd = kd;
    pc->up_limit = up_limit;
    pc->low_limit = low_limit;
}


//重置一个PID控制
void resetPidControl(PidControl* pc){
    pc->offset = 0;
    pc->sum_offset = 0;
    pc->dif_offset = 0;
}


//启动一个PID控制的一轮迭代，返回PID控制的输出值
float launchPidControl(PidControl* pc, float offset){
    //offset变号时，对积分部分清除
    if(pc->offset * offset < 0)
        pc->sum_offset = 0; 
    else pc->sum_offset += offset;
    pc->dif_offset = offset - pc->offset;
    float i_part = pc->ki * pc->sum_offset;
    //对积分部分约束
    if(i_part > I_MAX_RATE * pc->up_limit) i_part = I_MAX_RATE * pc->up_limit;
    else if(i_part < I_MAX_RATE * pc->low_limit) i_part = I_MAX_RATE * pc->low_limit;

    float output = pc->kp * offset + i_part + pc->kd * pc->dif_offset;
    pc->offset = offset;
    if(output > pc->up_limit) return pc->up_limit;
    if(output < pc->low_limit) return pc->low_limit;
    return output;
}

//机器人的PID控制表
PidControl robot_pid_table[ROBOT_NUM][PID_NUM];


//执行-冲突-PID，在游戏开始时，全局的初始化工作
void initPidGameStart(){
    for(int i=0;i<ROBOT_NUM;i++){
        initPidControlTable(&(robot_pid_table[i][0]), 3.5, 1.5, 0.35, ORIENT_LIMIT, -ORIENT_LIMIT); 
        initPidControlTable(&(robot_pid_table[i][1]), 4.5, 0.5, 1, SPEED_UP_LIMIT, SPEED_LOW_LIMIT); //TODO调参数
        initPidControlTable(&(robot_pid_table[i][2]), 0.5, 0.001, 0.05, SPEED_UP_LIMIT, SPEED_LOW_LIMIT);
    }
} 

//执行-冲突-PID，在运动阶段开始时，单个机器人的重置工作
void resetPidStageStart(int robot_id, int pid_id){
    resetPidControl(&(robot_pid_table[robot_id][pid_id]));
}


//执行-冲突-PID，在运动阶段执行时，单个机器人的运转工作
float launchPidStageExecute(int robot_id, int pid_id, float offset){
    float outcome = launchPidControl(&(robot_pid_table[robot_id][pid_id]), offset);
    if(robot_id == 2 && pid_id == 1){
        cerr<<current_frame<<" PID "<<robot_pid_table[robot_id][pid_id].offset<<" "<<robot_pid_table[robot_id][pid_id].sum_offset<<" "<<robot_pid_table[robot_id][pid_id].dif_offset<<" "<<outcome<<endl; 
    }
    return outcome;
}