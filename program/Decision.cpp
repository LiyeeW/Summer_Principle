#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"
#include "task.h"

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
    //TODO
    addDecision(0,0,6);
}

//输出选择
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}

//工作台间联系表
StationRelation station_relation_table[STATION_MAX_NUM][STATION_MAX_NUM];

//任务队列
list<TaskInfo> waiting_task_list;

//地图初始化：工作台联系表 和 任务队列
//初始任务队列的权重 = 剩余生产时间+差价/两地的距离
void initMap(void){
    for(int i=0;i<station_num;i++){
        for(int j=0;j<station_num;j++){
            switch (station_info_table[i].type)
            {
            case 1:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=3000;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+3000/station_relation_table[i][j].distance);
                }
                break;
            }
            case 2:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==6 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=3200;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+3200/station_relation_table[i][j].distance);
                }
                break;
            }
            case 3:
            {
                if(station_info_table[j].type==6 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=3400;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+3400/station_relation_table[i][j].distance);
                }
                break;
            }
            case 4:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=22500-15400;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+7100/station_relation_table[i][j].distance);
                }
                break;
            }
            case 5:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=25000-17200;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+7800/station_relation_table[i][j].distance);
                }
                break;
            }
            case 6:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=27500-19200;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+8300/station_relation_table[i][j].distance);
                }
                break;
            }
            case 7:
            {
                if(station_info_table[j].type==8 || station_info_table[j].type==9){
                    station_relation_table[i][j].value=105000-76000;
                    station_relation_table[i][j].distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,station_info_table[i].time+29000/station_relation_table[i][j].distance);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    sortTaskList();
}
