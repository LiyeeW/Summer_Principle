#ifndef move_trace_h
#define move_trace_h

#include "Robot.h"
#include "Movement.h"

using namespace std; 


//机器人半径统一估计为0.55m
const float ROBOT_RADIUS = 0.55;

//匀速圆周运动的半径统一为2m
const float ROTATE_RADIUS = 2;

//根号三
const float SQRT_THREE = 1.7321;

//当dx接近或等于0，计算所得的K的上界，为的是能用统一公式表示线段
const float MAX_K = 1000;


//机器人在stage3直线移动时，从当前位置到目标工作台的线段信息
typedef struct {
    float xs;       //起点x坐标，起点是机器人当前所在位置
    float ys;       //起点y坐标
    float xd;       //终点x坐标，终点是目标工作台的位置
    float yd;       //终点y坐标
    float k;        //线段的斜率，即tan(theta)=dy/dx
    float b;        //线段的截距，即y-Kx
} RobotTrace;


//机器人轨迹信息表
extern RobotTrace robot_trace_table[ROBOT_NUM];


//在某机器人朝新的目的地出发前，对轨迹记录的重置工作
void resetTraceBeforeDepart(int robot_id);

//两个机器人的线段是否有交点
bool isTraceCross(int robot_id1, int robot_id2);
//计算两个机器人的交点坐标和夹角
void updateMeetingInfo(int robot_id1, int robot_id2, float &x,float &y,float &orident);
//更新每帧的机器人轨迹信息表
void updateMovetracePerframe();
//更新robot_id的轨迹信息表
void updateMovetracePerframe(int robot_id);

//判断某（静止的）机器人是否在某（移动的）机器人的trace附近（线与圆相交）
//bool getOnRobotTrace(int still_robot, int move_robot);

 //判断机器人a是否在机器人b的trace附近（b的线与a的二倍半径圆相交）,a静止
bool getOnRobotTrace(int a, int b);

 //判断机器人a是否在机器人b的trace附近（b的线与a的index倍半径圆相交）,a是still
bool getOnRobotTrace(int a, int b, float index);

#endif