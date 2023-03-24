#ifndef station_h
#define station_h

#include <math.h>

//最大工作台数量
const int STATION_MAX_NUM = 50;
//工作台数量
extern int station_num;
//4~7产品数量
extern int product_num;

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

//统计各种产品当前的数量 1~7
extern int product_info_table[10];

//新增一个工作台信息
void addStationInfo(int type, float x, float y);

//更新一个工作台信息
void updateStationInfo(int id, int time, int raw, int ok);

//计算两个工作台的直线距离
float cacuStationDis(int id1, int id2);

//工作台之间的task_id，以表示任务间的依赖
typedef struct{
	int task_id;
}StationTaskRelation;

//工作台间联系表
extern StationTaskRelation station_task_table[STATION_MAX_NUM][STATION_MAX_NUM];

//添加i->j的任务编号为task_id
void addStationTask(int s,int d,int task_id);

//获取生产时间
int getTimeOfStation(int id);

//获取产品格状态
int getOkOfStation(int id);
//获取原材料格状态
int getRawOfStation(int id);

//获取工作台状态
int getTypeOfStation(int id);

//获取工作台剩余生产帧数
int getWaitFrameOfStation(int id);

//获取工作站x坐标
float getStationAxisX(int station_id);

//获取工作站y坐标
float getStationAxisY(int station_id);

//判断工作站的原材料格是否已满
bool isStaionFullRow(int station_id);

//更新产品统计,返回4~7产品总数
void updateProductInfo();

#endif