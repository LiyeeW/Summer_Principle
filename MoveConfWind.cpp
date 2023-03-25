#include "MoveConfWind.h"
#include "MoveConf.h"
#include <cmath>


namespace MoveConfWind{

//初始化
void init(RobotConf* confp){
    STAGE_STILL[LOCAL_TYPE][1][0] = false;  //role0全速直行，role1急刹车等待
    STAGE_STILL[LOCAL_TYPE][1][0] = true;   
}

//识别函数
void recognize(RobotConf* confp){
    // if(!confp->across || getPairFlat(confp)) return;
    // if(getPairAcrossDistSum(confp) > getPairWinDist(confp)) return;
    // setConfType(confp, LOCAL_TYPE);
    // //评估：(夹角余弦值+1)/双方到交点之和
    // setConfAssess(confp,((float)cos(confp->orient)+1)/getPairAcrossDistSum(confp)); 
}

//检查退出
void checkout(RobotConf* confp){
    // if(getPairAcrossDistSum(confp) > getPairWinDist(confp)){
    //     setConfStage(confp, -1);
    // }
}

//重置函数，在确定开始解决该冲突时的重置工作
void reset(RobotConf* confp){
    // if(getPairAcrossDist(confp, confp->role[0]) < getPairAcrossDist(confp, confp->role[1])){
    //     setConfRole(confp, confp->role[0]); //谁距离交点近谁是role0
    // }
    // else setConfRole(confp, confp->role[1]);
    // setConfStage(confp, 1);
    // resetPidStageStart(confp->role[0], 2);    //重置线速度PID
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){

}

//该冲突的状态机执行函数
void execute(RobotConf* confp){
    switch(confp->stage){
        case 1:{    //role0全速通过，role1急刹车
            setRobotNextOmega(confp->role[1], 0);
            setRobotNextSpeed(confp->role[1], launchPidStageExecute(confp->role[1], 0, getRobotSpeed(confp->role[1]))); 
            setRobotNextOmega(confp->role[0], 0);
            setRobotNextSpeed(confp->role[0], SPEED_UP_LIMIT);
            break;
        }
    }
}

}