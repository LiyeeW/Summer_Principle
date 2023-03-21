#include "MoveStage.h"
#include "Movement.h"
#include "Robot.h"
#include "Station.h"
#include "task.h"
#include "Score.h"
#include <cmath>
#include <iostream>

using namespace std;


 
//(每一帧)更新运动阶段
void updateMoveStage(int robot_id){
    switch(getRobotMoveStage(robot_id)){
        case 1:{
            //一旦检测到角度偏离，则跳转到stage 2
            if(!getRobotOrientLockedAccurate(robot_id)){
                setRobotDestPass(robot_id, false);
                setRobotMoveStage(robot_id, 2);
            }
            break;
        }
        case 2:{
            //如果方向精确锁定，根据是否需要等待调整到1或3
            if(getRobotOrientLockedAccurate(robot_id)){
                //如果临近目标并且目标需要等待，则调整到1
                if(getRobotApproached(robot_id) && getRobotDestWait(robot_id)){
                    setRobotMoveStage(robot_id, 1);
                }
                else{
                    setRobotMoveStage(robot_id, 3);
                }
            }
            break;
        }
        case 3:{
            //如果临近目标
            if(getRobotApproached(robot_id)){
                //如果方向已锁定
                if(getRobotOrientLocked(robot_id)){
                    //如果目的地需要等待，则原地等待
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
        case 4:{
            //如果不需要再等待，则跳回3进行综合判断
            if(getRobotDestWait(robot_id)){
                setRobotMoveStage(robot_id, 3);
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
    }else if(task_status==1){
    //需要增加对收货工作台的等待判定，用到raw信息    TODO
        int s = getSourceOfTask(robot_info_table[robot_id].task_id);
        int type = station_info_table[s].type;
        int d = getRobotDest(robot_id);
        if((station_info_table[d].raw & (1<<type)) == 0){
            setRobotDestWait(robot_id, false);
        }
    }
    
}


//运动系统必要的全局初始化工作
void initMoveGlobal(){
    for(int i=0;i<ROBOT_NUM;i++){
        initPidControlTable(&(robot_move_table[i].pidOrient), 3.5, 1.5, 0.35, ORIENT_LIMIT, -ORIENT_LIMIT); 
        initPidControlTable(&(robot_move_table[i].pidDistance), 0.5, 0.001, 0.05, DIRECT_UP_LIMIT, DIRECT_LOW_LIMIT);
    }
}


//在某机器人朝新的目的地出发前，对运动系统的重置工作
void resetMoveBeforeDepart(int robot_id){
    //重置运动阶段为2
    setRobotMoveStage(robot_id, 2);
    //重置目的地
    resetRobotDest(robot_id);
    //重置直线方向，初始直线偏角记录全部大于PI，便于后续通过新旧差值突变判断passover
    setRobotDestOrient(robot_id, 2*PI);
    //重置摇摆帧号
    setRobotLastSwingFrame(robot_id, current_frame);
    //重置未越过
    setRobotDestPass(robot_id, false);
    //重置是否需要等待，暂时按送货/取货区分
    //更新：初始化为均需要等待
    setRobotDestWait(robot_id, true);
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
    if(robot_id==3){
        //cerr<<current_frame<<" stage "<<getRobotMoveStage(robot_id)<<" near:"<<getRobotApproached(robot_id)<<" orientOffset "<<getRobotDestOrientOffset(robot_id);
        //cerr<<" distance "<<robot_move_table[robot_id].pidDistance.offset<<" "<<robot_move_table[robot_id].pidDistance.sum_offset<<" "<<robot_move_table[robot_id].pidDistance.dif_offset<<" set speed to "<<getRobotNextSpeed(robot_id)<<endl;
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
            setRobotNextSpeed(robot_id, getAverageSpeed(robot_id)); 
            // if(current_frame<5000 && robot_id == 1){
            //     cerr<<"when "<<current_frame<<" robot "<<robot_id<<" set omega to "<<getRobotNextOmega(robot_id)<<endl;
            //     cerr<<robot_move_table[robot_id].pidOrient.offset<<" "<<robot_move_table[robot_id].pidOrient.sum_offset<<" "<<robot_move_table[robot_id].pidOrient.dif_offset<<endl;
            // }
            break;
        }
        case 4:{
            float speed = launchPidControl(&(robot_move_table[robot_id].pidDistance), getRobotDestDistance(robot_id));
            setRobotNextSpeed(robot_id, speed);
            setRobotNextOmega(robot_id, 0);
            break;
        }
    }
}
