#include "MoveConf.h"
#include "Robot.h"
#include "MoveTrace.h"
#include<iostream>
#include<math.h>

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
    return robot_solve_table[robot_id];
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
    //先更新冲突信息表
    for(int i=0;i<ROBOT_NUM;i++){
        for(int j=i+1;j<ROBOT_NUM;j++){
            robot_conf_table[i][j].distance = getDistance(i,j);
            robot_conf_table[i][j].across = isTraceCross(i,j);
            if(robot_conf_table[i][j].across){
                updateMeetingInfo(i,j,robot_conf_table[i][j].x,robot_conf_table[i][j].y,robot_conf_table[i][j].orient);
            }
        }
    }
}

//是否角度算平
bool getPairFlat(RobotConf* confp){
    return radianLessThan(confp->orient,FLAT_ANGLE,true);
}

//是否方向相向
bool getPairOppo(RobotConf* confp){
    return confp->orient > PI/2;
}


//获取其中一方到交点的距离
float getPairAcrossDist(RobotConf* confp, int robot_id){
    float dx1 = robot_trace_table[robot_id].xs - confp->x;
    float dy1 = robot_trace_table[robot_id].ys - confp->y;
    return sqrt(dx1*dx1+dy1*dy1);
}

//获取双方到交点的距离之和
float getPairAcrossDistSum(RobotConf* confp){
    int id1 = (confp->role)[0];
    int id2 = (confp->role)[1];
    return getPairAcrossDist(confp, id1) + getPairAcrossDist(confp, id2);
}

//设定robot_id为新的role[0]，剩余的那个机器人编号为新的role[1]
void setConfRole(RobotConf* confp, int robot_id){
    if(confp->role[0]!=robot_id){
        confp->role[1] = confp->role[0];
        confp->role[0] = robot_id;
    }
}

//获得窗口长度
float getPairWinDist(RobotConf* confp){
    return WINDOWS_MARGIN * 4 * ROBOT_RADIUS / sin(confp->orient);
}

//返回在夹角中线的偏逆时针方向的一方的机器人编号
int getPairAntiOne(RobotConf* confp){
    int id1 = confp->role[0],id2 = confp->role[1];
    int o1 = getRobotDestOrient(id1), o2 = getRobotDestOrient(id2);
    if(o1*o2>=0){
        if(o2>o1)   return id2;
        else return id1;
    }else if(o1>0 && o2<0){
        //分为02是否超过o1的延长线
        if(o1-o2 > PI)  return id2;
        else return id1; 
    }else{
        if(o2-o1 > PI) return id1;
        else return id2;
    }
    
}

