#include "Movement.h"
#include "Robot.h"
#include "Station.h"
#include "task.h"
#include "Score.h"
#include <cmath>
#include <iostream>

//用于区分机器人和工作站的编号
int ROB(int robot_id){
    return robot_id - ROBOT_NUM;
}

//获取某个点的坐标，int参数表示编号，-4对应机器人0，-1对应机器人3
void getAxis(int a, float &y, float &x){
    if(a<0){
        a += ROBOT_NUM;
        y = getRobotAxisY(a), x = getRobotAxisX(a);
    }
    else{
        y = getStationAxisY(a), x = getStationAxisX(a);
    }
}

//获取两点的坐标差值，计划从a到b
void getAxisDiff(int a, int b, float& dy, float& dx){
    float y1, y2, x1, x2;
    getAxis(a, y1, x1), getAxis(b, y2, x2);
    dy = y2 - y1;
    dx = x2 - x1;
    
}

// 获取两点之间的距离，始终为正数
float getDistance(int a, int b){
    float dy, dx;
    getAxisDiff(a, b, dy, dx);
    return sqrt(dy*dy+dx*dx);
}

// 获取两点间的角度，计划从a到b
float getOrient(int a, int b){
    float dy, dx;
    getAxisDiff(a, b, dy, dx);
    //假设垂直
    float orient = 0.5*PI;
    //如果垂直且dy为负
    if(abs(dx) < TAN_X_MIN && dy<0) orient = -orient;
    else{
        //算得[-0.5PI,0.5PI]范围的角度
        orient = (float)atan(dy/dx);
        //根据非0的dx扩大到[-PI,PI]范围的角度
        if(dx<0) orient+=(dy<0)?-PI:PI;
    }
    return orient;  
}

//获取两个角度值的差，计划从o1转向o2
float getOrientDiff(float o1, float o2){
    float offset = o2 - o1;
    //收束到[-PI,PI]范围的角度
    if(offset<-PI) return offset+2*PI;
    if(offset>PI) return offset-2*PI;
    return offset;
}

//time以秒为单位，转为帧数
int timeToFrame(float time){
    return (int)(time/FRAME_SECOND);
}

//功能函数：返回s1到s2的直线全速花费帧数
int getStationsFrameCostDirect(int s1, int s2){
    return timeToFrame(getDistance(s1, s2)/DIRECT_UP_LIMIT);
} 

//功能函数：返回s1到s2的估计总花费帧数
int getStationsFrameCost(int s1, int s2){
    return getStationsFrameCostDirect(s1, s2) + ORIENT_FRAME_COST + OTHER_FRAME_COST;
}

//功能函数：返回s1到s2的估计总花费帧数，并且机器人是从s0到的s1
int getStationsFrameCost(int s1, int s2, int s0){
    float orient_diff = getOrientDiff(getOrient(s0, s1), getOrient(s1, s2));
    return getStationsFrameCostDirect(s1, s2) + timeToFrame(abs(orient_diff)/ORIENT_LIMIT) + OTHER_FRAME_COST;
}

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

//获取机器人的目的工作台的x坐标
float getRobotDestAxisX(int robot_id){
    return getStationAxisX(getRobotDest(robot_id));
}

//获取机器人的目的工作台的y坐标
float getRobotDestAxisY(int robot_id){
    return getStationAxisY(getRobotDest(robot_id));
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
    return getOrientDiff(robot_info_table[robot_id].orient, getRobotDestOrient(robot_id));
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

//在阶段三时，通过预估匀速到达需要等待的地点
float getAverageSpeed(int robot_id){
    //如果目的地不用等待，则全速
    if(!getRobotDestWait(robot_id)) return DIRECT_UP_LIMIT;
    float second = FRAME_SECOND*getWaitFrameOfStation(getRobotDest(robot_id));
    if(second<0 || second==0) return DIRECT_UP_LIMIT;
    return getRobotDestDistance(robot_id)/second; 
}

//(每一帧)更新某机器人的到目的地的直线方向、锁定、距离、靠近、越过等信息
void updateRobotDestDirect(int robot_id){
    int dest_station = getRobotDest(robot_id);
    //更新直线距离
    setRobotDestDistance(robot_id, getDistance(ROB(robot_id), dest_station));
    //计算角度
    float orient = getOrient(ROB(robot_id), dest_station);
    //如果不是第一次记录朝向，并且处于阶段一，才会考虑越过
    if(getRobotDestOrient(robot_id) < 1.5*PI && getRobotMoveStage(robot_id) == 1){
        //考虑浮点计算精度，若突变量约为PI，则越过
        if(abs(abs(orient - getRobotDestOrient(robot_id))-PI) < FLOAT_MARGIN){
            flipRobotDestPass(robot_id);
        }
    }
    setRobotDestOrient(robot_id,orient);
    updateRobotLastSwingFrame(robot_id);
}

//(每一帧)更新机器人是否需要在临近目的地时降速，等待生产
void updateRobotDestWait(int robot_id){
    //一旦不用等待，就不会再变回需要等待
    if(!getRobotDestWait(robot_id)) return;
    //如果已生产完毕
    int task_status = robot_info_table[robot_id].task_status;
    if(task_status==0){
        if(getOkOfStation(getRobotDest(robot_id)) == 1){
            setRobotDestWait(robot_id, false);
        }
    }
    else if(task_status==1){
    //需要增加对收货工作台的等待判定，用到raw信息    TODO
        int s = getSourceOfTask(robot_info_table[robot_id].task_id);
        int type = station_info_table[s].type;
        int d = getRobotDest(robot_id);
        if((station_info_table[d].raw & (1<<type)) == 0){
            setRobotDestWait(robot_id, false);
        }
    }    
}
