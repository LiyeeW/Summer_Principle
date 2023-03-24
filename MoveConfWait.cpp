#include "MoveConf.h"
#include "MoveConfWait.h"

namespace MoveConfWait { 



//初始化
void init(RobotConf* confp){
    STAGE_STILL[LOCAL_TYPE][1][0] = true;
}


//识别函数
void recognize(RobotConf* confp){

}

//检查退出
void checkout(RobotConf* confp){

}

//重置函数，在确定开始解决该冲突时的重置工作
//通过solving判断谁等谁
//上述有误，必须清楚等的谁
void reset(RobotConf* confp){

}

//该冲突的状态机切换函数
void jump(RobotConf* confp){

}

//该冲突的状态机执行函数
void execute(RobotConf* confp){

}

}; 