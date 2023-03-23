#include "MoveConf.h"
#include "Robot.h"
#include "MoveTrace.h"
#include "iostream"


using namespace std; 


//机器人之间的冲突信息表
RobotConf robot_conf_table[ROBOT_NUM][ROBOT_NUM];

//机器人的冲突解决信息表
RobotSolve robot_solve_table[ROBOT_NUM]; 


 


//获取机器人解决冲突的对象
int getConfSolvePartner(int robot_id){
    return robot_solve_table[robot_id].partner;
}

//设置机器人解决冲突的对象
void setConfSolvePartner(int robot_id, int partner){
    robot_solve_table[robot_id].partner = partner;
}

//获取机器人解决冲突的角色
int getConfSolveRole(int robot_id){
    return robot_solve_table[robot_id].role;
}

//设置机器人解决冲突的角色
void setConfSolveRole(int robot_id, int role){
    robot_solve_table[robot_id].role = role;
}

//读取冲突类型
int getConfType(int a, int b){
   return robot_conf_table[a][b].type;
}

//设置冲突类型
void setConfType(int a, int b, int type){
    robot_conf_table[a][b].type = type;
}

//设置冲突评估值
void setConfAssess(int a, int b, float assess){
    robot_conf_table[a][b].assess = assess;
}

//读取冲突阶段
int getConfStage(int a, int b){
    return robot_conf_table[a][b].stage;
}

//设置冲突阶段
void setConfStage(int a, int b, int stage){
    robot_conf_table[a][b].stage = stage;
    robot_conf_table[b][a].stage = stage;
}


//获取机器人是否正在解决冲突
bool getConfSolving(int robot_id){
    return robot_solve_table[robot_id].solving;
}

//设置机器人是否正在解决冲突
void setConfSolving(int robot_id, bool solving){
    robot_solve_table[robot_id].solving= solving;
}

//获取机器人当前正在处理的冲突信息
RobotConf* getRobotSolvingConf(int robot_id){
    if(!getConfSolving(robot_id)) return nullptr;
    int partner = getConfSolvePartner(robot_id);
    if(partner > robot_id) return &(robot_conf_table[robot_id][partner]);
    return &(robot_conf_table[partner][robot_id]);
}

//获取机器人是否静止
bool getConfStill(int robot_id){
    return robot_solve_table[robot_id].still;
}

//设置机器人是否静止
void setConfStill(int robot_id, bool still){
    robot_solve_table[robot_id].still = still;
}

//避障系统必要的全局初始化工作
void initMoveConfGlobal(){
    for(int a=0;a<ROBOT_NUM;a++){
        for(int b=a+1;b<ROBOT_NUM;b++){
            //状态机类型的初始值是没有冲突
            //robot_conf_table[a][b].type = -1;
        }
    }
}

//在每一帧运动执行前，更新全局的冲突信息，包含信息和冲突状态机的更新
void updateMoveConf();

//(每一帧)检查并执行机器人的可能的冲突解决，返回false时，调用者会继续执行基础的运动系统
bool checkMoveConf(int robot_id);