#include "PidControl.h"
#include <cmath>

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