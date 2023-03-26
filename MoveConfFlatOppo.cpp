#include "MoveConfFlatOppo.h"
#include "MoveTrace.h"
#include "MoveConf.h"
#include <cmath>

namespace MoveConfFlatOppo{

//TODO: 初始化
void init(RobotConf* confp){
    for(int i=0;i<ROLE_NUM;i++){
        STAGE_STILL[LOCAL_TYPE][1][i] = false;
    }
}

//识别函数
//TODO:role需要在识别时暂时分配，便于冲突集查找对应的机器人
void recognize(RobotConf* confp){
    //必须双方运动中
    if(getStageStill(confp->role[0]) || getStageStill(confp->role[1])) return;
    //识别标准：有交点，角度平，相向
    if(!confp->across || !getPairFlat(confp) || !getPairOppo(confp)) return;
    setConfType(confp, LOCAL_TYPE);
    //评估：(夹角余弦值+1)/双方到交点之和
    setConfAssess(confp,((float)cos(confp->orient)+1)/getPairAcrossDistSum(confp)); 
}

//检查退出
void checkout(RobotConf* confp){
    //退出标准：无交点，或者角度不平
    if(!confp->across || !getPairFlat(confp)){
        setConfStage(confp, -1);
    }
}

//重置函数，在确定开始解决该冲突时的重置工作
void reset(RobotConf* confp){
    setConfStage(confp, 1);
    //TODO:计算确定同时往哪个方向转，暂时都直接逆时针
    //TODO:计算旋转时的外轮廓圆弧能否拉满，确定旋转时的线速度
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){
    //stage 1：如果两者轮廓带没有相交，或者两者夹角达到max，则退出（大概率回到常规的stage 2）
}

//该冲突的状态机执行函数
void execute(RobotConf* confp){
    switch(confp->stage){
        case 1:{    //stage 1:匀速圆周运动
            for(int r=0;r<ROLE_NUM;r++){
                setRobotNextOmega(confp->role[r], ROTATE_OMEGA);
                setRobotNextSpeed(confp->role[r], SPEED_UP_LIMIT); 
            }
        }
    }
}



};