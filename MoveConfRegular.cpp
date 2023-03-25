#include "Score.h"
#include "PidControl.h"
#include "Movement.h"
#include "MoveConf.h"
#include "MoveConfRegular.h"
#include <iostream>



namespace MoveConfRegular{







//跳转到某个stage时需要做的重置工作
//regular中的RobotConf采用的[i][i]
bool jumpTo(RobotConf* confp, int stage){
    //设定conf的stage
    setConfStage(confp, stage);
    //获取robot_id
    int robot_id = confp->role[0];
    switch(stage){
        case 1:{
            setConsiderPass(robot_id, true);
            resetPidStageStart(robot_id, 1);    //重置距离PID
            break;
        }
        case 2:{
            setConsiderPass(robot_id, false);
            setRobotDestPass(robot_id, false);  //设置没有越过
            setRobotDestOrient(robot_id, getOrient(ROB(robot_id), getRobotDest(robot_id)));//-1时刻的直线偏角也要有，后续通过新旧差值突变判断passover
            setRobotLastSwingFrame(robot_id, current_frame);   //重置摇摆帧号
            resetPidStageStart(robot_id, 0);    //重置角度PID
            break;
        }
        case 3:{
            setConsiderPass(robot_id, false);            
            break;
        }
    }
    return true;
}



//TODO: 初始化，参数是个摆设
void init(RobotConf* confp){
    for(int i=0;i<ROLE_NUM;i++){
        STAGE_STILL[LOCAL_TYPE][1][i] = true;
        STAGE_STILL[LOCAL_TYPE][2][i] = true;
        STAGE_STILL[LOCAL_TYPE][3][i] = false;
    }
}

//识别：0表示不冲突
void recognize(RobotConf* confp){
    setConfType(confp, LOCAL_TYPE);
    //评估：恒为不紧急
    setConfAssess(confp,0);
}

//检查退出
void checkout(RobotConf* confp){
    //常规type没有退出
}

//重置函数，重置该状态机
void reset(RobotConf* confp){   //初始stage为2，不区分role
    jumpTo(confp, 2);
}




//该冲突的状态机切换函数
bool jumpLoop(RobotConf* confp){
    //获取robot_id
    int robot_id = confp->role[0];
    switch(confp->stage){
        case 1:{
            //一旦检测到角度偏离，则跳转到stage 2
            if(!getRobotOrientLockedAccurate(robot_id)) return jumpTo(confp, 2);
            //TODO当前状态机的退出时机的识别
            break;
        }
        case 2:{
            //如果方向精确锁定
            if(getRobotOrientLockedAccurate(robot_id)){
                //如果临近目标，无论目标是否需要等待，都调整到stage 1；没有临近则调整到3；
                if(getRobotApproached(robot_id)) return jumpTo(confp, 1);
                else return jumpTo(confp, 3);
            }
            break;
        }
        case 3:{
            //如果临近目标，则跳转到stage 2
            if(getRobotApproached(robot_id)) return jumpTo(confp, 2);
            break;
        }
    }
    return false;
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){
    while(jumpLoop(confp));
}


//该冲突的状态机执行函数
void execute(RobotConf* confp){
    //获取robot_id
    int robot_id = confp->role[0];
    if(robot_id==2){
        cerr<<confp->stage<<" ";
    }
    switch(confp->stage){
        case 1:{
            float speed = launchPidStageExecute(robot_id, 1, getRobotDestDistance(robot_id));  
            setRobotNextSpeed(robot_id, speed);
            setRobotNextOmega(robot_id, 0);
            break;
        }
        case 2:{
            float omega = launchPidStageExecute(robot_id, 0, getRobotDestOrientOffset(robot_id));
            setRobotNextOmega(robot_id, omega);
            setRobotNextSpeed(robot_id, 0);
            break;
        }
        case 3:{
            float omega = launchPidStageExecute(robot_id, 0, getRobotDestOrientOffset(robot_id));
            setRobotNextOmega(robot_id, omega);
            setRobotNextSpeed(robot_id, getAverageSpeed(robot_id)); 
            break;
        }
    }
}


}