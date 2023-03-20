#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"
#include "task.h"
#include "Bid.h"

#include <cstring>
#include <iostream>

using namespace std;

//做出选择
void makeDecision(){
    //如果出现空闲机器人：
    //0. （如果有任务完成）标记已完成的任务为free
    //1. 生成能获取的任务id列表
    //2. 为空闲机器人生成bids
    //3. 为空闲机器人分配task
    
    //将已完成的任务状态更新
    for(int i=0;i<robot_num;i++){
        if(robot_info_table[i].task_status == 2){
            //当有机器人robot_id完成任务后，更新任务状态
            tagFreeTask(getTaskofRobot(i));
            //标记机器人状态空闲
            addTasktoRobot(i,-1,-1);
        }
    }
    bool flag = false;
    for(int i=0;i<robot_num;i++){  
        //为每个空闲机器人生成bids：（没有出现空闲机器人：先不管）
        if(robot_info_table[i].task_status == -1){
            if(!flag){
                updateAvailList();   //初始时应该只有1，2，3生产工作台对应的任务
                flag = true;
            }
            generateBids(i);
        }
    }
    assignTaskfromBids();
    //end
    
}


//判断当前任务是否冲突，即不能两个robot前往同一个source取货，或前往同一个dest卖货
bool isTaskConflict(int taskid){
    int s = getSourceOfTask(taskid);
    int d = getDestOfTask(taskid);
    //判断冲突
    for(int i=0;i<robot_num;i++){
        //当前处于任务执行状态
        if(robot_info_table[i].task_id != -1){
            if(robot_info_table[i].task_status == 0 && s == getSourceOfTask(robot_info_table[i].task_id)) return true;   //source冲突
            if(d == getDestOfTask(robot_info_table[i].task_id) && station_info_table[d].type<8) return true;  //dest冲突
        }
    }
    return false;
}

//计算具有最大price的任务并返回
void assignTaskfromBids(){
    //前提保证：不空闲的robot对应的bids_list为空
    float max_price;
    int max_robot_id;
    while(1){
        max_price = 0;
        max_robot_id = -1;
        //bids_list暴露出来
        for(int i=0;i<robot_num;i++){
            if(i==1){
                cerr<<bids_list[1][0].task_id<<" "<<bids_list[1][0].price<<endl;
            }
            if(!bids_list[i].empty()){
                //不能分配相同的任务给两个robot，此外，不能两个robot前往同一个source取货，或前往同一个dest卖货
                while(isTaskBusy(bids_list[i][0].task_id) || isTaskConflict(bids_list[i][0].task_id)){
                    bids_list[i].erase(bids_list[i].begin());
                }
                if(max_price<bids_list[i][0].price){
                    max_price = bids_list[i][0].price;
                    max_robot_id = i;
                }
            }
        }
        if(max_robot_id != -1){
            //分配任务给竞标成功的robot，并清空其bids_list
            addTasktoRobot(max_robot_id,bids_list[max_robot_id][0].task_id,0);
            //debug
            int taskid = bids_list[max_robot_id][0].task_id;
            int s = waiting_task_list[taskid].source, d = waiting_task_list[taskid].dest;
            std::cerr<<"assign task to robot "<<max_robot_id<<",s is "<<s<<",d is "<<d<<endl;
            cerr<<"s: "<<station_info_table[s].x<<" "<<station_info_table[s].y<<" "<<station_info_table[s].type<<endl;
            cerr<<"d: "<<station_info_table[d].x<<" "<<station_info_table[d].y<<" "<<station_info_table[d].type<<endl;
            //
            tagBusyTask(bids_list[max_robot_id][0].task_id);
            clearBidList(max_robot_id);
        }else{
            break;
        }    
    }
}

//遍历所有任务，找出能前往的任务id放到avail_taskid_list
void updateAvailList(void){
    clearTaskIdList();  //先清空能做的所有任务
    for(int i=0;i<task_num;i++){
        int s = getSourceOfTask(i);
        int d = getDestOfTask(i);
        //source必须已经有了产品 或者 处于生产中
        if(getTimeOfStation(s)==-1 && getOkOfStation(s)==0){
            continue;
        }
        //dest必须有位置放产品 暂时不考虑dest正在生产的情况
        int type = getTypeOfStation(s);
        int raw = getRawOfStation(d);
        if(((1<<type) & raw) != 0){
            continue;
        }
        //当前id加入
        addTaskId(i);
    }
    if(isEmptyAvaiList()){
        cerr<<"---------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!bug:no availtasklist"<<endl;
    }
}

//为robot_id生成报价并排序，暂定的报价=差价/(机器人到生产节点的距离+工作台之间的距离)
void generateBids(int robot_id){
    clearBidList(robot_id);
    //将avail_taskid_list暴露
    for(int task_id: avail_taskid_list){
        float xx = robot_info_table[robot_id].x-station_info_table[waiting_task_list[task_id].source].x;
        float yy = robot_info_table[robot_id].y-station_info_table[waiting_task_list[task_id].source].y;
        //报价需要进一步精确化 TODO
        addBidInfo(robot_id,task_id,waiting_task_list[task_id].value/(sqrt(xx*xx+yy*yy)+waiting_task_list[task_id].distance));
    }
    sortBidList(robot_id);
}

//地图初始化：生成所有任务
//初始任务队列的权重 = 差价/两地的距离
void initMap(void){
    //generate all tasks:
    for(int i=0;i<station_num;i++){
        for(int j=0;j<station_num;j++){
            switch (station_info_table[i].type)
            {
            case 1:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 2:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==6 || station_info_table[j].type==9){
                    int value=3200;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 3:
            {
                if(station_info_table[j].type==6 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3400;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 4:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7100;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 5:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7800;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 6:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=8300;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 7:
            {
                if(station_info_table[j].type==8 || station_info_table[j].type==9){
                    int value=29000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    std::cerr<<"initmap: total "<<task_num<<" tasks"<<endl;
    cerr<<"total robotnum: "<<robot_num<<" robots"<<endl;
}
