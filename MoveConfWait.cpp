#include "MoveConf.h"
#include "MoveConfWait.h"

namespace MoveConfWait { 

//为了区分等对方冲突解决，还是等对方动
bool wait_move = false;


//初始化
void init(RobotConf* confp){
    STAGE_STILL[LOCAL_TYPE][1][0] = true;
    STAGE_STILL[LOCAL_TYPE][2][0] = true;
}


//识别函数
void recognize(RobotConf* confp){
    wait_move = false;
    //无法从内部识别wait
    //TODO:如果离得近（2-3m），如果一方运动，一方静止，并且静止方在运动方轨迹上
    //评估为很紧急:两点距离
    wait_move = true;
}

//检查退出
void checkout(RobotConf* confp){
    int partner = confp->role[1];
    if(confp->stage == 1 && getConfSolving(partner) == nullptr){
        setConfStage(confp, -1);
    }
    else if(confp->stage == 2 && !getStageStill(partner)){
        setConfStage(confp, -1);
    }
}

//重置函数，在确定开始解决该冲突时的重置工作
//通过solving判断谁等谁
//上述有误，必须清楚等的谁
void reset(RobotConf* confp){
    //用stage 稳定区分
    if(wait_move) setConfStage(confp, 2);
    else setConfStage(confp, 1);
    resetPidStageStart(confp->role[0], 2);    //重置线速度PID
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){

}

//该冲突的状态机执行函数
void execute(RobotConf* confp){
    int robot_id = confp->role[0];
    switch(confp->stage){
        case 1:
        case 2:
        {    //急刹车
            setRobotNextOmega(robot_id, 0);
            setRobotNextSpeed(robot_id, launchPidStageExecute(robot_id, 2, -getRobotSpeed(robot_id))); 
            break;
        }
    }
}

}; 