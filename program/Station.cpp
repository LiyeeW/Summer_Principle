#include "Station.h"

using namespace std;

//工作台数量
int station_num;
//工作台位置信息表
StationInfo station_info_table[STATION_MAX_NUM];

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
}

//计算两个工作台的直线距离
float cacuStationDis(int id1, int id2){
    float disx = station_info_table[id1].x-station_info_table[id2].x;
    float disy = station_info_table[id2].y-station_info_table[id2].y;
    return sqrt(disx*disx+disy*disy);
}
