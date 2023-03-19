#include "PidControl.h"

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
    pc->sum_offset += offset;
    pc->dif_offset = offset - pc->offset;
    float output = pc->kp * offset + pc->ki * pc->sum_offset + pc->kd * pc->dif_offset;
    pc->offset = offset;
    if(output > pc->up_limit) return pc->up_limit;
    if(output < pc->low_limit) return pc->low_limit;
    return output;
}