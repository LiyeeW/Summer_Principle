#ifndef station_h
#define station_h

#include <math.h>

//最大工作台数量
const int STATION_MAX_NUM = 50;
//工作台数量
extern int station_num;

//工作台的便于距离计算的关键信息
typedef struct {
	int type;	//1-9的工作台类型
	float x;	//x坐标
	float y;	//y坐标
	int time;	//剩余生产时间
	int raw;	//原材料格状态
	int ok;		//产品格状态
} StationInfo;


//工作台位置信息表
extern StationInfo station_info_table[STATION_MAX_NUM];

//新增一个工作台信息
void addStationInfo(int type, float x, float y);

//更新一个工作台信息
void updateStationInfo(int id, int time, int raw, int ok);

//计算两个工作台的直线距离
float cacuStationDis(int id1, int id2);

//工作台之间的联系
typedef struct{
	float value; //收益
	float distance; //距离
}StationRelation;

//工作台间联系表
extern StationRelation station_relation_table[STATION_MAX_NUM][STATION_MAX_NUM];

#endif