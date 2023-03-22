#include "MoveConf.h"
#include "Robot.h"
#include "MoveTrace.h"
#include "iostream"


using namespace std; 


//机器人之间的冲突信息表
RobotConf robot_conf_table[ROBOT_NUM][ROBOT_NUM];


//避障系统必要的全局初始化工作
void initMoveConfGlobal(){
    for(int a=0;a<ROBOT_NUM;a++){
        for(int b=a+1;b<ROBOT_NUM;b++){
            //状态机类型的初始值是没有冲突
            robot_conf_table[a][b].type = -1;
        }
    }
}

//在每一帧运动执行前，更新全局的冲突信息，包含信息和冲突状态机的更新
void updateMoveConf();

//(每一帧)检查并执行机器人的可能的冲突解决，返回false时，调用者会继续执行基础的运动系统
bool checkMoveConf(int robot_id);