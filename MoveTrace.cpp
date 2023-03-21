#include "MoveTrace.h"
#include "Movement.h"


//在某机器人朝新的目的地出发前，对轨迹记录的重置工作
void resetTraceBeforeDepart(int robot_id){
    robot_trace_table[robot_id].xd = getRobotDestAxisX(robot_id);
    robot_trace_table[robot_id].yd = getRobotDestAxisY(robot_id);
}