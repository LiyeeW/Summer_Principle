#include "MoveConfFlatSame.h"
#include "MoveConf.h"

namespace MoveConfFlatSame{

//本地冲突类型
const int LOCAL_TYPE = 4;

//本地stage容量（一般0空出来）
const int LOCAL_STAGE_NUM = 2;

//角色0的方向，为1是顺时针，为-1是逆时针
int role0_direction = 1;



//初始化
void init(RobotConf* confp){
    for(int i=0;i<ROLE_NUM;i++){
        STAGE_STILL[LOCAL_TYPE][1][i] = false;
    }
}


//识别函数
void recognize(RobotConf* confp){
    //识别标准：有交点，角度平，同向
    if(!confp->across || !getPairFlat(confp) || getPairOppo(confp)) return;
    setConfType(confp, LOCAL_TYPE);
    //评估：1/双方距离
    setConfAssess(confp,1/confp->distance); 
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
    //role0_direction = (getPairAntiOne(confp)==conf->role[0])?1:-1;
}

//该冲突的状态机切换函数
void jump(RobotConf* confp){

}

//该冲突的状态机执行函数
void execute(RobotConf* confp){
    switch(confp->stage){
        case 1:{    //stage 1:匀速圆周运动
            setRobotNextOmega(confp->role[0], role0_direction * ROTATE_OMEGA);
            setRobotNextSpeed(confp->role[0], SPEED_UP_LIMIT); 
            setRobotNextOmega(confp->role[1], -role0_direction * ROTATE_OMEGA);
            setRobotNextSpeed(confp->role[1], SPEED_UP_LIMIT); 
        }
    }
}

}
