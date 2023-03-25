#include "Station.h"
#include<iostream>
#include"Score.h"

using namespace std;

//工作台数量
int station_num;
//工作台位置信息表
StationInfo station_info_table[STATION_MAX_NUM];
//4~7产品数量
int product_num;

//统计各种产品当前的数量 1~7
int product_info_table[10];

//新增一个工作台信息
void addStationInfo(int type, float x, float y){
    station_info_table[station_num].type = type;
    station_info_table[station_num].x = x;
    station_info_table[station_num++].y = y;
}

//更新一个工作台信息
void updateStationInfo(int id, int time, int raw, int ok){
    station_info_table[id].time = time;
    station_info_table[id].raw = raw;
    station_info_table[id].ok = ok;
    //if(current_frame<2) cerr<<current_frame<<" "<<id<<" "<<time<<" "<<ok<<endl;
}

//计算两个工作台的直线距离
float cacuStationDis(int id1, int id2){
    float disx = station_info_table[id1].x-station_info_table[id2].x;
    float disy = station_info_table[id1].y-station_info_table[id2].y;
    return sqrt(disx*disx+disy*disy);
}

//工作台间联系表
StationTaskRelation station_task_table[STATION_MAX_NUM][STATION_MAX_NUM];

//添加i->j的任务编号为task_id
void addStationTask(int s,int d,int task_id){
    station_task_table[s][d].task_id = task_id;
}

//获取生产时间
int getTimeOfStation(int id){
	return station_info_table[id].time;
}

//获取产品格状态
int getOkOfStation(int id){
	return station_info_table[id].ok;
}

//获取原材料格状态
int getRawOfStation(int id){
	return station_info_table[id].raw;
}

//获取工作台状态
int getTypeOfStation(int id){
	return station_info_table[id].type;
}

//获取工作台剩余生产帧数
int getWaitFrameOfStation(int id){
    return station_info_table[id].time;
}

//获取工作站x坐标
float getStationAxisX(int station_id){
    return station_info_table[station_id].x;
}

//获取工作站y坐标
float getStationAxisY(int station_id){
    return station_info_table[station_id].y;
}

//判断工作站的原材料格是否已满
//对于1，2，3（不需要原料的）和8，9（工作帧数为1），直接返回false
bool isStaionFullRow(int station_id){
    int raw = getRawOfStation(station_id); 
    bool isflag = false;   //为true表示原材料格已满
    switch(getTypeOfStation(station_id)){  //4~7
        case 4:{
            isflag = raw == ((1<<1)+(1<<2)); 
            break;
        }
        case 5:{
            isflag = raw == ((1<<1)+(1<<3));
            break;
        }
        case 6:{
            isflag = raw == ((1<<2)+(1<<3));
            break;
        }
        case 7:{
            isflag = raw == ((1<<4)+(1<<5)+(1<<6));
            break;
        }
        default:{
            break;
        }
    }
    return false;
}

//更新产品统计,返回4~7产品总数
void updateProductInfo(){
    for(int i=4;i<=7;i++){
        product_info_table[i]=0;
    }
    product_num = 0;
    for(int i=0;i<station_num;i++){
        if(getOkOfStation(i)==1 && getTypeOfStation(i)>=4 && getTypeOfStation(i)<=7){
            product_num++;
            product_info_table[getTypeOfStation(i)]++;
        }
    }
}
