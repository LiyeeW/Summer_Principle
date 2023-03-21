#include "Movement.h"
#include "Robot.h"
#include "Station.h"
#include "task.h"
#include "Score.h"
#include <cmath>
#include <iostream>


//机器人运动信息表
RobotMove robot_move_table[ROBOT_NUM];



//修改机器人运动阶段
void setRobotMoveStage(int robot_id, int stage){
    robot_move_table[robot_id].stage = stage;
}

//获取机器人运动阶段
int getRobotMoveStage(int robot_id){
    return robot_move_table[robot_id].stage;
}

//修改机器人的目标工作台
void setRobotDest(int robot_id, int station_id){
    robot_move_table[robot_id].destStation = station_id;
}

//获取机器人的目标工作台
int getRobotDest(int robot_id){
    return robot_move_table[robot_id].destStation;
}

//重置某机器人的目标工作台，此时task_status只可能是0或1
void resetRobotDest(int robot_id){
    int task_id = getTaskofRobot(robot_id);
    int task_status = robot_info_table[robot_id].task_status;
    // 如果正在取货，目的地是task的source
    if(task_status == 0){
        setRobotDest(robot_id, getSourceOfTask(task_id));
    }
    // 如果正在送货，目的地是task的dest
    else if(task_status == 1){
        setRobotDest(robot_id, getDestOfTask(task_id));
    }
    else cerr<<"movement: robot "<<robot_id<<" task status "<<task_status<<endl;
}

//修改机器人到目标工作台的直线朝向
void setRobotDestOrient(int robot_id, float orient){
    robot_move_table[robot_id].destOrient = orient;
}

//获取机器人到目标工作台的直线朝向
float getRobotDestOrient(int robot_id){
    return robot_move_table[robot_id].destOrient;
}

//获取机器人到目标工作台的直线朝向-机器人实际朝向的差量
float getRobotDestOrientOffset(int robot_id){
    float offset = getRobotDestOrient(robot_id) - robot_info_table[robot_id].orient;
    if(offset<-PI) return offset+2*PI;
    if(offset>PI) return offset-2*PI;
    return offset;
}

//判断机器人的方向和角速度是否满足要求
bool getRobotSwing(int robot_id){
    if(abs(getRobotDestOrientOffset(robot_id)) > LOCK_ORIENT) return true;
    //if(abs(robot_info_table[robot_id].omega) > LOCK_OMEGA) return true;
    return false;
}

//修改上一次方向和角速度不满足的帧号
void setRobotLastSwingFrame(int robot_id, int frame){
    robot_move_table[robot_id].lastSwingFrame = frame;
}

//获取上一次方向和角速度不满足的帧号
int getRobotLastSwingFrame(int robot_id){
    return robot_move_table[robot_id].lastSwingFrame;
}

//如果当前帧仍在摇摆，记录下来
void updateRobotLastSwingFrame(int robot_id){
    if(getRobotSwing(robot_id)) setRobotLastSwingFrame(robot_id, current_frame);
}

//获取机器人是否大体调向成功，能够加速
bool getRobotOrientLocked(int robot_id){
    return current_frame >= getRobotLastSwingFrame(robot_id) + LOCK_FRAME;
}

//a，b均为正的浮点数，比较在[-PI,PI]范围内，a是否近似小于b，用于阶段一的近距离差角判断
bool radianLessThan(float a, float b){
    if(a<b) return true;
    if(abs(a-PI)<b) return true;
    else return false;
}

//获取机器人是否精确调向成功，能够直线移动
bool getRobotOrientLockedAccurate(int robot_id){
    float offset = abs(getRobotDestOrientOffset(robot_id));
    return radianLessThan(offset, ORIENT_LOST);
}

//修改机器人是否相比出发时已越过目的地
void setRobotDestPass(int robot_id, bool pass){
    robot_move_table[robot_id].destPass = pass;
}

//获取机器人是否相比出发时已越过目的地
bool getRobotDestPass(int robot_id){
    return robot_move_table[robot_id].destPass;
}

//反转机器人是否相比出发时已越过目的地
void flipRobotDestPass(int robot_id){
    robot_move_table[robot_id].destPass = !getRobotDestPass(robot_id);
}

//修改机器人到目标工作台的直线距离
void setRobotDestDistance(int robot_id, float distance){
    robot_move_table[robot_id].destDistance = distance;
}

//获取机器人到目标工作台的直线距离，若已越过，则返回负值
float getRobotDestDistance(int robot_id){
    float rtn = robot_move_table[robot_id].destDistance;
    return (getRobotDestPass(robot_id))?-rtn:rtn;
}

//修改目的地是否需要等待
void setRobotDestWait(int robot_id, bool wait){
    robot_move_table[robot_id].destWait = wait;
}

//获取目的地是否需要等待
bool getRobotDestWait(int robot_id){
    return robot_move_table[robot_id].destWait;
}

//获取机器人是否临近目的地，此时距离不可能为负
bool getRobotApproached(int robot_id){
    return getRobotDestDistance(robot_id) <= APPROACH_DISTANCE;
}

//修改机器人下一步的角速度
void setRobotNextOmega(int robot_id, float omega){
    robot_move_table[robot_id].nextOmega = omega;
}

//获取机器人下一步的角速度
float getRobotNextOmega(int robot_id){
    return robot_move_table[robot_id].nextOmega;
}

//修改机器人下一步的线速度
void setRobotNextSpeed(int robot_id, float speed){
    robot_move_table[robot_id].nextSpeed = speed;
}

//获取机器人下一步的线速度
float getRobotNextSpeed(int robot_id){
    return robot_move_table[robot_id].nextSpeed;
}

float getAverageSpeed(int robot_id){
    //如果目的地不用等待，则全速
    if(!getRobotDestWait(robot_id)) return DIRECT_UP_LIMIT;
    float second = FRAME_SECOND*getWaitFrameOfStation(getRobotDest(robot_id));
    if(second<0 || second==0) return DIRECT_UP_LIMIT;
    return getRobotDestDistance(robot_id)/second; 
}

