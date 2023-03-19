#include "Movement.h"
#include "Robot.h"
#include "Station.h"
#include "task.h"
#include "Score.h"
#include <cmath>
#include <iostream>


//机器人运动信息表
RobotMove robot_move_table[ROBOT_NUM];


//(OK)之后，在每次切换目标地点时，需要调用resetPID
//(TODO)在获取到每一帧数据后，需要调用launchPID
//(TODO)PID的参数需要结合判题器做实验，根据经验人为设置
//(TODO)需要先设定速度，再设定角速度，因为当速度为负时，实际朝向会加减一个PI




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
    if(offset<-PI) return offset+PI;
    if(offset>PI) return offset-PI;
    return offset;
}

//判断机器人的方向和角速度是否满足要求
bool getRobotSwing(int robot_id){
    if(abs(getRobotDestOrientOffset(robot_id)) > LOCK_ORIENT) return true;
    if(abs(robot_info_table[robot_id].omega) > LOCK_OMEGA) return true;
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

//获取机器人是否锁定方向，返回动态判断结果
bool getRobotOrientLocked(int robot_id){
    return current_frame >= getRobotLastSwingFrame(robot_id) + LOCK_FRAME;
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



//(每一帧)更新运动阶段
void updateMoveStage(int robot_id){
    switch(getRobotMoveStage(robot_id)){
        case 1:{
            //在完成当前目的之前，保持在阶段一
            break;
        }
        case 2:{
            //如果方向已完成锁定，根据是否需要等待调整到1或3
            if(getRobotOrientLocked(robot_id)){
                setRobotMoveStage(robot_id, (getRobotDestWait(robot_id))?1:3);
            }
            break;
        }
        case 3:{
            //如果临近目标
            if(getRobotApproached(robot_id)){
                //如果方向已锁定
                if(getRobotOrientLocked(robot_id)){
                    //如果目的地需要等待
                    if(getRobotDestWait(robot_id)){
                        setRobotMoveStage(robot_id, 1);
                    }
                    //如果不需要等待则保持为3
                }
                //如果方向未锁定，无论是否需要等待，都调整到2
                else{
                    setRobotMoveStage(robot_id, 2);
                }
            }
            break;
        }
    }
}

//(每一帧)更新某机器人的到目的地的直线方向、锁定、距离、靠近、越过等信息
void updateRobotDestDirect(int robot_id){
    int station_id = getRobotDest(robot_id);
    float dy = station_info_table[station_id].y - robot_info_table[robot_id].y;
    float dx = station_info_table[station_id].x - robot_info_table[robot_id].x;
    setRobotDestDistance(robot_id, sqrt(dy*dy+dx*dx));
    float orient;
    if(abs(dx)<0.0001) orient = (dy<0)?-0.5*PI:0.5*PI;
    else{
        orient = (float)atan(dy/dx);
        if(dx<0) orient+=(dy<0)?-PI:PI;
    }    
    //如果不是第一次记录朝向，并且处于阶段一，才会考虑越过
    if(getRobotDestOrient(robot_id) < 1.5*PI && getRobotMoveStage(robot_id) == 1){
        //考虑浮点计算精度，若突变量约为PI，则越过
        if(abs(orient - getRobotDestOrient(robot_id)) > 0.9 * PI){
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
    if(getOkOfStation(getRobotDest(robot_id)) == 1){
        setRobotDestWait(robot_id, false);
    }
}


//运动系统必要的全局初始化工作
void initMoveGlobal(){
    for(int i=0;i<ROBOT_NUM;i++){
        initPidControlTable(&(robot_move_table[i].pidOrient), 0.55, 0.01, 0.05, ORIENT_LIMIT, -ORIENT_LIMIT); 
        initPidControlTable(&(robot_move_table[i].pidDistance), 0.5, 0.001, 0.05, DIRECT_UP_LIMIT, DIRECT_LOW_LIMIT);
    }
}


//在某机器人朝新的目的地出发前，对运动系统的重置工作
void resetMoveBeforeDepart(int robot_id){
    //重置运动阶段为3
    setRobotMoveStage(robot_id, 3);
    //重置目的地
    resetRobotDest(robot_id);
    //重置直线方向，初始直线偏角记录全部大于PI，便于后续通过新旧差值突变判断passover
    setRobotDestOrient(robot_id, 2*PI);
    //重置摇摆帧号
    setRobotLastSwingFrame(robot_id, current_frame);
    //重置未越过
    setRobotDestPass(robot_id, false);
    //重置是否需要等待，暂时按送货/取货区分
    setRobotDestWait(robot_id, getTaskStatusofRobot(robot_id)==0);
    //重置PID控制
    resetPidControl(&(robot_move_table[robot_id].pidOrient));
    //TODO：两种控制的开始与结束周期不一定是按目的地切换为划分
    resetPidControl(&(robot_move_table[robot_id].pidDistance));
}


//每帧更新运动记录，包括直线角度与距离、是否需要等待、运动阶段等
void updateMovePerFrame(){
    for(int i=0;i<ROBOT_NUM;i++){
        //更新等待情况
        updateRobotDestWait(i);
        //更新直线方向、锁定、距离、靠近、越过等信息
        updateRobotDestDirect(i);
        //更新运动阶段
        updateMoveStage(i);

    }
}

//根据运动阶段和PID算法得到下一步的角速度和线速度
void moveByStage(int robot_id){
    if(robot_id==1){
        cerr<<current_frame<<" "<<getRobotMoveStage(robot_id)<<" near:"<<getRobotApproached(robot_id)<<" "<<getRobotDestDistance(robot_id);
        cerr<<" omega "<<robot_move_table[robot_id].pidOrient.offset<<" "<<robot_move_table[robot_id].pidOrient.sum_offset<<" "<<robot_move_table[robot_id].pidOrient.dif_offset<<" set omega to "<<getRobotNextOmega(robot_id)<<endl;
        //cerr<<getRobotDestOrientOffset(robot_id)<<" "<<getRobotDestDistance(robot_id)<<endl;
        //cerr<<robot_move_table[robot_id].pidDistance.offset<<" "<<robot_move_table[robot_id].pidDistance.sum_offset<<" "<<robot_move_table[robot_id].pidDistance.dif_offset<<" set speed to "<<getRobotNextSpeed(robot_id)<<endl;
    }
    //if(current_frame<20) 
    switch(getRobotMoveStage(robot_id)){
        case 1:{
            float speed = launchPidControl(&(robot_move_table[robot_id].pidDistance), getRobotDestDistance(robot_id));
            setRobotNextSpeed(robot_id, speed);
            setRobotNextOmega(robot_id, 0);
            break;
        }
        case 2:{
            float omega = launchPidControl(&(robot_move_table[robot_id].pidOrient), getRobotDestOrientOffset(robot_id));
            setRobotNextOmega(robot_id, omega);
            setRobotNextSpeed(robot_id, 0);
            break;
        }
        case 3:{
            float omega = launchPidControl(&(robot_move_table[robot_id].pidOrient), getRobotDestOrientOffset(robot_id));
            setRobotNextOmega(robot_id, omega);
            setRobotNextSpeed(robot_id, DIRECT_UP_LIMIT);
            // if(current_frame<5000 && robot_id == 1){
            //     cerr<<"when "<<current_frame<<" robot "<<robot_id<<" set omega to "<<getRobotNextOmega(robot_id)<<endl;
            //     cerr<<robot_move_table[robot_id].pidOrient.offset<<" "<<robot_move_table[robot_id].pidOrient.sum_offset<<" "<<robot_move_table[robot_id].pidOrient.dif_offset<<endl;
            // }
            break;
        }
    }
}
