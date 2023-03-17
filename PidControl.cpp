#include "PidControl.h"

//初始化一组PID控制
void initPidControlTable(int num, PidControl* pc, float kp, float ki, float kd, float up_limit, float low_limit){
    for(int i=0;i<num;i++){
        pc[i].kp = kp;
        pc[i].ki = ki;
        pc[i].kd = kd;
        pc[i].up_limit = pc[i].low_limit;
    }
}


//重置一个PID控制
void resetPidControl(int id, PidControl* pc){
    pc[id].offset = 0;
    pc[id].sum_offset = 0;
    pc[id].dif_offset = 0;
}


//启动一个PID控制的一轮迭代，返回PID控制的输出值
float launchPidControl(int id, PidControl* pc, float offset){
    pc[id].sum_offset += offset;
    pc[id].dif_offset = offset - pc[id].offset;
    float output = pc[id].kp * offset + pc[id].ki * pc[id].sum_offset + pc[id].kd * pc[id].dif_offset;
    pc[id].offset = offset;
    if(output > pc[id].up_limit) return pc[id].up_limit;
    if(output < pc[id].low_limit) return pc[id].low_limit;
    return output;
}