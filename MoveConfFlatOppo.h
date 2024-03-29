#ifndef move_conf_flat_oppo_h
#define move_conf_flat_oppo_h
#include "Movement.h"
#include "MoveConf.h"

namespace MoveConfFlatOppo { 


//

//相向夹角在多少以内算平
const float FLAT_ANGLE = PI/6;

//旋转时得到的最大夹角
const float MAX_ROTATE_ANGLE = PI*0.4;

//对窗口距离的裕度放宽倍数
const float WINDOWS_MARGIN = 1.2;

//本地冲突类型
const int LOCAL_TYPE = 1;

//本地stage容量（一般0空出来）
const int LOCAL_STAGE_NUM = 3;



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


}; 

#endif