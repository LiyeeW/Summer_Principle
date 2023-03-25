#ifndef move_conf_all_h
#define move_conf_all_h
#include "MoveConf.h"

//执行-冲突中，游戏开始时，全局要做的初始化
void initConfGameStart();

//重置冲突
void resetConfBeforeDepart(int robot_id);

//执行-冲突中，每一帧，数据更新
void updateMoveconfPerframe(int robot_id);

//执行-冲突中，每一帧，数据更新
void updateMoveconfPerframe();

//执行-冲突
void executeConf();

//冲突方案的基础函数的共同签名，便于使用函数指针数组
typedef void(*ConFun)(RobotConf* confp);

//识别任何可能存在的冲突，将识别结果
void recognizeNewConf();

#endif