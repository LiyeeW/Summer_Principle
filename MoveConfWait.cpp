#include "MoveConf.h"
#include "MoveConfWait.h"

namespace MoveConfWait { 



//初始化
void init(RobotConf* confp){
    STAGE_STILL[LOCAL_TYPE][1][0] = true;
}


//识别函数
void recognize(RobotConf* confp){
    //无法从内部识别wait
}

//检查退出
void checkout(RobotConf* confp){
    int partner = confp->role[1];
    if(getConfSolving(partner) == nullptr){
        setConfStage(confp, -1);
    }
}

//重置函数，在确定开始解决该冲突时的重置工作
//通过solving判断谁等谁
//上述有误，必须清楚等的谁
void reset(RobotConf* confp){
    setConfStage(confp, 1);
    resetPidStageStart(confp->role[0], 2);    //重置线速度PID
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){

}

//该冲突的状态机执行函数
void execute(RobotConf* confp){
    int robot_id = confp->role[0];
    switch(confp->stage){
        case 1:{    //急刹车
            setRobotNextOmega(robot_id, 0);
            setRobotNextSpeed(robot_id, launchPidStageExecute(robot_id, 0, getRobotSpeed(robot_id))); 
            break;
        }
    }
}

}; 