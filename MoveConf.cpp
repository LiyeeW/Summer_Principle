#include "MoveConf.h"
#include "Robot.h"
#include "MoveTrace.h"
#include "iostream"


using namespace std; 


//机器人之间的冲突信息表
RobotConf robot_conf_table[ROBOT_NUM][ROBOT_NUM];

//机器人的冲突解决信息表
RobotConf* robot_solve_table[ROBOT_NUM];

//静止参考表
bool STAGE_STILL[CONF_TYPE_NUM][MAX_STAGE_NUM][ROLE_NUM];

//获取两个机器人之间的冲突对
RobotConf* getConfPair(int a, int b){
    if(a<b) return &(robot_conf_table[a][b]);
    return &(robot_conf_table[b][a]);
}

//获取机器人正在解决的冲突
RobotConf* getConfSolving(int robot_id){
    return robot_solve_table[robot_id];
}

//获取机器人正在解决的冲突，返回值不能为nullptr
RobotConf* getConfSolving(int robot_id, bool must){
    if(getConfSolving(robot_id) == nullptr) return &(robot_conf_table[robot_id][robot_id]);
}

//设置机器人正在解决的冲突
void setConfSolving(int robot_id, RobotConf* confp){
    robot_solve_table[robot_id] = confp;
}

//读取冲突类型
int getConfType(int robot_id){
    return getConfSolving(robot_id, true)->type;
}

//读取冲突阶段
int getConfStage(int robot_id){
    return getConfSolving(robot_id, true)->stage;
}

//获取冲突角色
int getConfRole(int robot_id){
    RobotConf* confp = getConfSolving(robot_id, true);
    for(int i=0;i<ROLE_NUM;i++){
        if(confp->role[i]==robot_id) return i;
    }
    return -1;  //ISBUG
}

//获取机器人是否静止
bool getStageStill(int robot_id){
    int confType = getConfType(robot_id);
    int confStage = getConfStage(robot_id); 
    int confRole = getConfRole(robot_id);
    return STAGE_STILL[confType][confStage][confRole];
}

//设置冲突类型
void setConfType(RobotConf* confp, int type){
    confp->type = type;
}

//设置冲突评估值
void setConfAssess(RobotConf* confp, float assess){
    confp->assess = assess;
}


//设置冲突阶段
void setConfStage(RobotConf* confp, int stage){
    confp->stage = stage;
}


//执行-冲突-冲突对中，游戏开始时，全局的初始化工作
void initConfpairGameStart(){
    for(int a=0;a<ROBOT_NUM;a++){
        setConfSolving(a, nullptr);
        for(int b=a;b<ROBOT_NUM;b++){
            robot_conf_table[a][b].role[0] = a;
            robot_conf_table[a][b].role[1] = b;
        }
    }
}

//在每一帧运动执行前，更新全局的冲突信息，包含信息和冲突状态机的更新
void updateMoveConf(){

}


