#ifndef move_conf_flat_h
#define move_conf_flat_h
#include "Movement.h"
#include "MoveConf.h"


namespace MoveConfFlatSame{

//本地冲突类型
const int LOCAL_TYPE = 4;

//本地stage容量（一般0空出来）
const int LOCAL_STAGE_NUM = 2;



//初始化
void init(RobotConf* confp);


//识别函数
void recognize(RobotConf* confp);

//检查退出
void checkout(RobotConf* confp);

//重置函数，在确定开始解决该冲突时的重置工作
void reset(RobotConf* confp);

//该冲突的状态机切换函数
void jump(RobotConf* confp);

//该冲突的状态机执行函数
void execute(RobotConf* confp);

}


#endif