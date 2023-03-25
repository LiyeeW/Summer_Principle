#include "MoveTrace.h"
#include "Movement.h"
#include "Score.h"
#include <cmath>
#include <iostream>




//机器人轨迹信息表
RobotTrace robot_trace_table[ROBOT_NUM];

//在某机器人朝新的目的地出发前，对轨迹记录的重置工作
void resetTraceBeforeDepart(int robot_id){
    //更新终点坐标
    getAxis(getRobotDest(robot_id), robot_trace_table[robot_id].xd, robot_trace_table[robot_id].yd);
} 

//根据dy和dx计算k
float updateTraceLineK(int robot_id, float dy, float dx){
    //如果垂直，近似斜率为MAX_K
    if(dx==0) return MAX_K;
    float k = dy/dx;
    //接近垂直时，也近似为MAX_K
    if(abs(k)>MAX_K) return k/abs(k)*MAX_K;
    return k;
}

//根据起点坐标和k计算b
float updateTraceLineB(int robot_id){
    return robot_trace_table[robot_id].ys - robot_trace_table[robot_id].xs * robot_trace_table[robot_id].k;
}

//(每帧)更新线段信息
void updateTraceLine(int robot_id){
    //更新起点坐标
    getAxis(ROB(robot_id), robot_trace_table[robot_id].xs, robot_trace_table[robot_id].ys);
    //计算斜率和截距，终点减起点
    float dy = robot_trace_table[robot_id].yd - robot_trace_table[robot_id].ys;
    float dx = robot_trace_table[robot_id].xd - robot_trace_table[robot_id].xs;
    robot_trace_table[robot_id].k = updateTraceLineK(robot_id, dy, dx);
    robot_trace_table[robot_id].b = updateTraceLineB(robot_id);
}

