#include "Score.h"
#include "PidControl.h"
#include "MoveTrace.h"
#include "MoveConf.h"
#include "MoveConfAll.h"
#include "MoveConfRegular.h"
#include "MoveConfFlatOppo.h" 
#include "MoveConfWait.h"  
#include "MoveConfWind.h" 
#include "MoveConfFlatSame.h"
#include "Robot.h"
#include <iostream>

using namespace std;



//初始化
ConFun confInit[CONF_TYPE_NUM] = {MoveConfRegular::init, MoveConfFlatOppo::init, MoveConfWait::init, MoveConfWind::init, MoveConfFlatSame::init}; 
//识别 
ConFun confRecognize[CONF_TYPE_NUM] = {MoveConfRegular::recognize, MoveConfFlatOppo::recognize, MoveConfWait::recognize, MoveConfWind::recognize, MoveConfFlatSame::recognize};
//检查退出
ConFun confCheckout[CONF_TYPE_NUM] = {MoveConfRegular::checkout, MoveConfFlatOppo::checkout, MoveConfWait::checkout, MoveConfWind::checkout, MoveConfFlatSame::checkout};
//分配时的重置
ConFun confReset[CONF_TYPE_NUM] = {MoveConfRegular::reset, MoveConfFlatOppo::reset, MoveConfWait::reset, MoveConfWind::reset, MoveConfFlatSame::reset}; 
//跳转
ConFun confJump[CONF_TYPE_NUM] = {MoveConfRegular::jump, MoveConfFlatOppo::jump, MoveConfWait::jump, MoveConfWind::jump, MoveConfFlatSame::jump};
//执行
ConFun confExecute[CONF_TYPE_NUM] = {MoveConfRegular::execute, MoveConfFlatOppo::execute, MoveConfWait::execute, MoveConfWind::execute, MoveConfFlatSame::execute};


//冲突集，与并查集相似，记录当前集合的评分最高的冲突对
RobotConf* newConfSet[ROBOT_NUM];

//切换冲突类型
void switchConfType(RobotConf* confp){
    (*confReset[confp->type])(confp);
    if(confp->type!=MoveConfRegular::LOCAL_TYPE){
        setConfSolving(confp->role[0], confp);
        if(confp->type!=MoveConfWait::LOCAL_TYPE){
            setConfSolving(confp->role[1], confp);
        }
    }
    cerr<<" switchConfType "<<confp->role[0]<<" "<<confp->role[1]<<" "<<confp->type<<endl;
}


//清空冲突集
void resetNewConfSet(){
    for(int i=0;i<ROBOT_NUM;i++){
        newConfSet[i] = nullptr;
    }
}

//更新冲突集
void updateNewConfSet(RobotConf* confp){
    int a = confp->role[0], b= confp->role[1];
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
    RobotConf* rtn = nullptr;
    for(int i=0;i<ROBOT_NUM;i++){
        if(newConfSet[i] != nullptr){
            rtn = newConfSet[i];
            newConfSet[i] = nullptr;
            break;
        }
    }
    return rtn;
}


//通过分配WAIT的方式清除某冲突集中还未solving的记录
//等待对象为MAX冲突对的任一方
void eraseFromNewConfSet(RobotConf* confp_ing){
    for(int i=0;i<ROBOT_NUM;i++){
        if(newConfSet[i]!=confp_ing) continue;
        if(getConfSolving(i) == nullptr){
            RobotConf* confp = getConfPair(i, confp_ing->role[0]);
            setConfType(confp, MoveConfWait::LOCAL_TYPE); 
            setConfRole(confp, i);  //明确谁等谁
            cerr<<i<<" because1 "<<confp_ing->role[0]<<endl;
            switchConfType(confp);
        }
        newConfSet[i]=nullptr;
    }
}


//识别评估一对机器人之间任何可能存在的冲突，将识别结果写进冲突表
void recognizePairAnyConf(int a, int b){
    //如果有任何一方正在solving，那么在找到新冲突后，非solving的一方会被立刻分配为confWait
    int wait_id = (getConfSolving(b)!=nullptr)?a:(getConfSolving(a)!=nullptr)?b:-1;
    RobotConf* confp = getConfPair(a, b);
    for(int i=0;i<CONF_TYPE_NUM;i++){
        (*confRecognize[i])(confp); //识别
        if(confp->type!=MoveConfRegular::LOCAL_TYPE){//新找到一个冲突对；
            cerr<<"  find "<<confp->type<<" between "<<confp->role[0]<<" and "<<confp->role[1]<<endl;
            if(wait_id!=-1){ //立刻分配为confWait
                setConfType(confp, MoveConfWait::LOCAL_TYPE);
                setConfRole(confp, wait_id);
                cerr<<wait_id<<" because2 "<<confp->role[1]<<endl;
                switchConfType(confp);
            }
            else{ //加入冲突集
                updateNewConfSet(&(robot_conf_table[a][b]));
            }
            return; //TODO假设：一对之间最多只存在一种冲突
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


//新冲突的识别、评估、解析和分配
void recognizeNewConf(){
    resetNewConfSet();      //重置冲突集
    for(int a=0;a<ROBOT_NUM;a++){
        //TODO假设：一旦开始处理冲突，就不能切换到其他冲突，直至解决退出；后续可能设置一个紧急阈值，大于阈值可以插队切换
        for(int b=a+1;b<ROBOT_NUM;b++){
            //TODO：如果双方都在处理冲突，旧的不应该被重复设置，新的冲突也切换不了
            if(getConfSolving(b) != nullptr && getConfSolving(a) != nullptr) continue;
            //如果双方都静止，不会存在冲突
            if(getStageStill(a) && getStageStill(b)) continue;
            recognizePairAnyConf(a,b);
        }
    }
    assignByNewConfSet();
}



//冲突解决完成，冲突类型回到regular；在regular中，切换目的地也是一种完成
void finishConf(RobotConf* confp){
    for(int i=0;i<ROLE_NUM;i++){
        //WAIT不干扰对方
        if(confp->type == MoveConfWait::LOCAL_TYPE && i==1) continue;
        setConfSolving(confp->role[i], nullptr);
        RobotConf* confp_role = getConfSolving(confp->role[i], true);
        switchConfType(confp_role);
    }
    setConfType(confp, MoveConfRegular::LOCAL_TYPE);
    switchConfType(confp);
}
void finishConf(int robot_id){
    finishConf(getConfSolving(robot_id, true));
}

//旧冲突的识别和取消
bool checkoutFinishedConf(){
    bool done[ROBOT_NUM] = {false};
    for(int i=0;i<ROBOT_NUM;i++){
        RobotConf* confp = getConfSolving(i);
        if(confp == nullptr || done[i]) continue;
        (*confCheckout[confp->type])(confp);    //如果checkout成功，stage会变为-1
        if(confp->stage == -1){
            for(int r=0;r<ROLE_NUM;r++) done[confp->role[r]] = true;
            finishConf(confp);
        }
    }
    for(int i=0;i<ROBOT_NUM;i++){   //如果本轮checkout有成功的，可能再来一轮扫描会取消一个wait冲突
        if(done[i]) return true;
    }
    return false;
}



//机器人切换目的地后，冲突也需要重置
void resetConfBeforeDepart(int robot_id){
    finishConf(robot_id);
}

//执行-冲突中，每一帧，数据更新
void updateMoveconfPerframe(int robot_id){
    RobotConf* confp = getConfSolving(robot_id, true);
    (*confJump[confp->type])(confp);
}

//执行-冲突中，每一帧，数据更新
void updateMoveconfPerframe(){
    for(int i=0;i<ROBOT_NUM;i++){
        RobotConf* confp = getConfSolving(i, true);
        (*confJump[confp->type])(confp);
    }
    //更新全局冲突信息
    updateMoveConf();
}



//执行-冲突中，游戏开始时，全局要做的初始化
void initConfGameStart(){
    initPidGameStart();     //PID参数初始化
    initConfpairGameStart();    //冲突对的初始化
    for(int i=0;i<CONF_TYPE_NUM;i++){
        (*confInit[i])(nullptr);      //各个冲突类型的初始化
    }
    for(int i=0;i<ROBOT_NUM;i++){
        setConfType(&(robot_conf_table[i][i]), MoveConfRegular::LOCAL_TYPE);
    }
}



//执行-冲突
void executeConf(){
    while(checkoutFinishedConf()); //去掉刚解决的冲突
    recognizeNewConf();  //加上新发现的冲突
    bool done[ROBOT_NUM] = {false}; //标记已经执行过的，则会跳过
    for(int i=0;i<ROBOT_NUM;i++){
        if(done[i]) continue;
        RobotConf* confp = getConfSolving(i, true);
        (*confExecute[confp->type])(confp);
        if(i == DERO)
            cerr<<current_frame<<" conf "<<confp->type<<" "<<confp->stage<<" "<<confp->role[0]<<" "<<confp->role[1]<<endl;
        //wait是单方执行，不能标记对方为done
        if(confp->type != MoveConfWait::LOCAL_TYPE){
            for(int r=0;r<ROLE_NUM;r++) done[confp->role[r]] = true;
        }
    }
}


