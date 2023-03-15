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
    //1. 首先重新排序bids，已完成的task加入bid中
    //2. 分配任务给空闲机器人（这个计算可能超时）
    //从bids选择当前空闲机器人报价最高的任务进行分配，选中task被标记为忙碌，该task对应所有bid失效

    //初始测试：
    for(int i=0;i<robot_num;i++){
        for(BidInfo bid: bids_list){
            if(bid.robot_id == i){  //分配给i号机器人
                addDecision(0,i,6);
                break;
            }
        }
    }
    //end

    //没有出现空闲机器人：先不管

    //addDecision(0,0,6);
}

//输出选择
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}

//地图初始化：生成所有任务 以及 所有报价并排序 简单地分配任务
//初始任务队列的权重 = 剩余生产时间+差价/两地的距离
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
                }
                break;
            }
            case 2:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==6 || station_info_table[j].type==9){
                    int value=3200;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 3:
            {
                if(station_info_table[j].type==6 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3400;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 4:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7100;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 5:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7800;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 6:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=8300;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 7:
            {
                if(station_info_table[j].type==8 || station_info_table[j].type==9){
                    int value=29000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    //generate all bids:
    for(int i=0;i<robot_num;i++){
        for(int j=0;j<task_num;j++){
            float xx = robot_info_table[i].x-station_info_table[waiting_task_list[j].source].x;
            float yy = robot_info_table[i].y-station_info_table[waiting_task_list[j].source].y;
            addBidInfo(i,j,sqrt(xx+yy)); 
            //报价需要进一步精确化 TODO
        }
    }
    sortBidList();
    //简单地分配任务
}
