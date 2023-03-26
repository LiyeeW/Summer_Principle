#include "Execute.h"
#include "Station.h"
#include "Robot.h"
#include "Movement.h"
#include "MoveTrace.h"
#include "MoveConf.h"
#include "MoveConfAll.h" 
#include "task.h"
#include "Score.h"
#include <cstring>
#include <iostream>

using namespace std;





//执行类型对应的字符串
char execute_string[EXECUTE_TYPE_NUM][10] = {"forward", "rotate", "buy", "sell", "destroy"};
//执行信息表
ExecuteInfo execute_info_table[50];
//执行的数量
int execute_num = 0;
//标记本帧需要切换目的地
bool execute_switch[ROBOT_NUM];


//执行系统中，游戏开始前，全局需要做的初始化工作
void initExecuteGameStart(){
    for(int i=0;i<ROBOT_NUM;i++){
        execute_switch[i] = true;
    }
    initConfGameStart();
}

//执行系统中，拿到每帧传感器数据后，全局需要做的更新工作
void updateExecutePerFrame(int robot_id){
    updateMovementPerFrame(robot_id);
    updateMovetracePerframe(robot_id);  //TODO
    updateMoveconfPerframe(robot_id); 
}

//执行系统中，拿到每帧传感器数据后，全局需要做的更新工作
void updateExecutePerFrame(){
    updateMovementPerFrame();
    updateMovetracePerframe();  //TODO
    updateMoveconfPerframe();
}


//执行系统中，切换目的地后，单个机器人需要做的重置工作；TODO
void resetExecuteBeforeDepart(int robot_id){
    //重置切换标记
    execute_switch[robot_id] = false;
    //重置冲突的关键信息
    resetConfBeforeDepart(robot_id);
    //重置运动的关键信息
    resetMovementBeforeDepart(robot_id);
    //重置运动轨迹的关键信息
    resetTraceBeforeDepart(robot_id);
    cerr<<current_frame<<": robot "<<robot_id<<" dest to "<<getRobotDest(robot_id)<<endl;
}




//新增执行
void addExecute(int type, int robot, float param){
    execute_info_table[execute_num++] = {type, robot, param};
}


//到达目的地后的处理函数
void destArrivedHandler(int robot_id, int station_id){
    execute_switch[robot_id] = true;
    // 如果正在取货(执行到这已确定不用等待)
    if(getTaskStatusofRobot(robot_id) == 0){
        //购买生产好的产品
        addExecute(2,robot_id,0);
        //切换任务状态为送货
        setTaskStatusofRobot(robot_id,1);
        int taskid = getTaskofRobot(robot_id);
        int stationid = getSourceOfTask(taskid);
        int type = station_info_table[getSourceOfTask(taskid)].type;
        cerr<<current_frame<<" :robot "<<robot_id<<" buy from "<<stationid<<",type="<<type<<endl;
    }
    //如果正在送货
    else{
        //销售产品
        addExecute(3,robot_id,0);
        //切换任务状态为完成
        setTaskStatusofRobot(robot_id,2);
        //预更新 目的地 原材料格
        int taskid = getTaskofRobot(robot_id);
        int stationid = getDestOfTask(taskid);
        int type = station_info_table[getSourceOfTask(taskid)].type;
        cerr<<current_frame<<" :robot "<<robot_id<<", sell to "<<stationid<<", type="<<type<<endl;
        station_info_table[stationid].raw += (1<<type);
    }
}


//在运动执行前检查目标切换
void checkExecuteSwitch(){
    for(int robot_id=0; robot_id<ROBOT_NUM; robot_id++){
        if(!execute_switch[robot_id]) continue;
        //切换目标后，需要先重置关键信息，再常规更新信息
        resetExecuteBeforeDepart(robot_id);
        updateExecutePerFrame(robot_id);
    }
}

//制订本帧的销售执行方案
void executeTrade(){
    //清空上一轮的所有执行方案
    execute_num = 0;    
    for(int i=0;i<ROBOT_NUM;i++){
        int real_station = robot_info_table[i].station;
        int dest_station = getRobotDest(i);
        //如果已到达目的地，且不用等待，则执行到达处理
        if(real_station == dest_station && !getRobotDestWait(i)){
            destArrivedHandler(i,dest_station);
        }
        //
        if(real_station == dest_station && getRobotDestWait(i)){
            if(i==DERO) 
                cerr<<"*********************"<<current_frame<<": robot "<<i<<" wait to "<<real_station<<",raw="<<getRawOfStation(dest_station)<<"time="
            <<getTimeOfStation(dest_station)<<endl;
        }
    }
}

//制订本帧的兼容避障的运动执行方案
void executeMove(){
    //检查并执行可能的目的地切换
    checkExecuteSwitch();
    //执行冲突
    executeConf();
    for(int i=0;i<ROBOT_NUM;i++){
        addExecute(0,i,getRobotNextSpeed(i));
        addExecute(1,i,getRobotNextOmega(i));
    }
}


//输出执行方案
void outputExecute(){
    for(int i=0;i<execute_num;i++){
        printf("%s %d", execute_string[execute_info_table[i].type], execute_info_table[i].robot);
        if(execute_info_table[i].type<=1) printf(" %f\n", execute_info_table[i].param);
        else printf("\n");
    }
}
