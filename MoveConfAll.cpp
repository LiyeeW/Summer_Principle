#include "MoveTrace.h"
#include "MoveConf.h"
#include "MoveConfAll.h"
#include "MoveConfRegular.h"
#include "MoveConfFlatOppo.h" 
#include "MoveConfWait.h"  
#include "Robot.h"

using namespace std;



//初始化
ConFun confInit[CONF_TYPE_NUM] = {MoveConfRegular::init, MoveConfFlatOppo::init, MoveConfWait::init};
//识别
ConFun confRecognize[CONF_TYPE_NUM] = {MoveConfRegular::recognize, MoveConfFlatOppo::recognize, MoveConfWait::recognize};
//分配时的重置
ConFun confReset[CONF_TYPE_NUM] = {MoveConfRegular::reset, MoveConfFlatOppo::reset, MoveConfWait::reset}; 
//跳转
ConFun confLaunch[CONF_TYPE_NUM] = {MoveConfRegular::launch, MoveConfFlatOppo::launch, MoveConfWait::launch};
//执行
ConFun confExecute[CONF_TYPE_NUM] = {MoveConfRegular::execute, MoveConfFlatOppo::execute, MoveConfWait::execute};
//冲突集，与并查集相似，记录当前集合的评分最高的冲突对
RobotConf* newConfSet[ROBOT_NUM];


//切换冲突类型
void switchConfType(RobotConf* confp){
    int a = confp->role0, b= confp->role1;
    (*(confReset[confp->type]))(a, b);
}

//切换冲突类型
void switchConfType(int a, int b, int type){
    setConfType(a, b, type);
    (*(confReset[type]))(a, b);
}

//清空冲突集
void resetNewConfSet(){
    for(int i=0;i<ROBOT_NUM;i++){
        newConfSet[i] = nullptr;
    }
}

//更新冲突集
void updateNewConfSet(RobotConf* confp){
    int a = confp->role0, b= confp->role1;
    RobotConf* max = confp;
    if(newConfSet[a]!=nullptr && newConfSet[a]->assess > max->assess){
        max = newConfSet[a];
    }
    if(newConfSet[b]!=nullptr && newConfSet[b]->assess > max->assess){
        max = newConfSet[b];
    }
    newConfSet[a] = max, newConfSet[b] = max;
}

//获取冲突集中第一个非空的记录
RobotConf* getFromNewConfSet(){
    for(int i=0;i<ROBOT_NUM;i++){
        if(newConfSet[i] != nullptr) return newConfSet[i];
    }
    return nullptr;
}


//通过分配WAIT的方式清除某冲突集中还未solving的记录
//等待对象为MAX冲突对的任一方
RobotConf* eraseFromNewConfSet(RobotConf* confp){
    for(int i=0;i<ROBOT_NUM;i++){
        if(newConfSet[i]!=confp) continue;
        if(!getConfSolving(i)){
            switchConfType(i, confp->role1, MoveConfWait::LOCAL_TYPE);
        }
        newConfSet[i]=nullptr;
    }
}




//全局的冲突系统的初始化
void initConfGlobal(){
    for(int i=0;i<CONF_TYPE_NUM;i++){
        (*(confInit[i]))(0,0);
    }
}

//识别评估一对机器人之间任何可能存在的冲突，将识别结果写进冲突表
void recognizePairAnyConf(int a, int b){
    //如果有任何一方正在solving，那么在找到新冲突后，非solving的一方会被立刻分配为confWait
    bool mayWait = getConfSolving(b) || getConfSolving(a);
    for(int i=0;i<CONF_TYPE_NUM;i++){
        (*(confRecognize[i]))(a,b);
        //新找到一个冲突对；
        if(getConfType(a,b)!=0){
            //立刻分配为confWait
            if(mayWait){
                switchConfType(a, b, MoveConfWait::LOCAL_TYPE);
            }
            //加入冲突集
            else{
                updateNewConfSet(&(robot_conf_table[a][b]));
            }
            //TODO假设：一对之间最多只存在一种冲突
            return;
        }
    }
}

//根据冲突集分配冲突解决
void assignByNewConfSet(){
    RobotConf* confp = getFromNewConfSet();
    while(confp!=nullptr){
        switchConfType(confp);
        eraseFromNewConfSet(confp);
        confp = getFromNewConfSet();
    }
}

//更新冲突识别和评估
void recognizeAnyConf(){
    //重置冲突集
    resetNewConfSet();
    //TODO，有可能在这里更新trace和conf的基础计算；还没想清楚基础计算和状态机计算的先后关系
    for(int a=0;a<ROBOT_NUM;a++){
        //TODO假设：一旦开始处理冲突，就不能切换到其他冲突，直至解决退出；后续可能设置一个紧急阈值，大于阈值可以插队切换
        //if(getConfSolving(a)) continue;
        for(int b=a+1;b<ROBOT_NUM;b++){
            //TODO：如果双方都在处理冲突，旧的不应该被重复设置，新的冲突也切换不了
            if(getConfSolving(b) && getConfSolving(a)) continue;
            //如果双方都静止，不会存在冲突
            if(getConfStill(a) && getConfStill(b)) continue;
            recognizePairAnyConf(a,b);
        }
    }
    assignByNewConfSet();
}


//根据基础信息更新stage





