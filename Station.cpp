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
