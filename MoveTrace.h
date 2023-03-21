#ifndef move_trace_h
#define move_trace_h

#include "Robot.h"

using namespace std; 

typedef struct {
    float xs;       //起点x坐标，起点是机器人当前所在位置
    float ys;       //起点y坐标
    float xd;       //终点x坐标，终点是目标工作台的位置
    float yd;       //终点y坐标
    float lineK;    //线段的斜率，即tan(theta)=dy/dx
    float lineB;    //线段的截距，即y-Kx
    bool conflict;  //是否有冲突：几乎平行相交则不论距离；其他相交等相近时才冲突
    int status;     //仅在冲突时有效：1是平行相交中，2是其他相交且靠近中
    int who;        //仅在冲突时有效：-1表示冲突对象正在与别的机器人解决冲突，其他表示与之解决冲突的机器人编号
} RobotTrace;

//机器人轨迹信息表
extern RobotTrace robot_trace_table[ROBOT_NUM];

//在某机器人朝新的目的地出发前，对轨迹记录的重置工作
void resetTraceBeforeDepart(int robot_id);

#endif