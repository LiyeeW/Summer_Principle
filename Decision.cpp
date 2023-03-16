#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"
#include "task.h"
#include "Bid.h"

#include <cstring>
#include <iostream>

using namespace std;

//选择的数量
int decision_num = 0;


//选择类型对应的字符串
char decision_string[DECISION_TYPE_NUM][10] = {"forward", "rotate", "buy", "sell", "destroy"};
//选择信息表
DecisionInfo decision_info_table[50];


//新增选择
void addDecision(int type, int robot, float param){
    decision_info_table[decision_num++] = {type, robot, param};
}

//做出选择
void makeDecision(){
    //清空上一轮的选择
    decision_num = 0;

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

//输出选择
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
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
            if(!bids_list[i].empty()){
                while(isTaskBusy(bids_list[i][0].task_id)){
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
        if(((1<<(type-1)) & raw) == 0){
            continue;
        }
        //当前id加入
        addTaskId(i);
    }
}

//为robot_id生成报价并排序，暂定的报价=机器人到生产节点的距离+差价/工作台之间的距离
void generateBids(int robot_id){
    clearBidList(robot_id);
    //将avail_taskid_list暴露
    for(int task_id: avail_taskid_list){
        float xx = robot_info_table[robot_id].x-station_info_table[waiting_task_list[task_id].source].x;
        float yy = robot_info_table[robot_id].y-station_info_table[waiting_task_list[task_id].source].y;
        //报价需要进一步精确化 TODO
        addBidInfo(robot_id,task_id,sqrt(xx+yy)+getWeightOfTask(task_id));
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
}
