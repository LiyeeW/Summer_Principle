#include "Station.h"

using namespace std;

//����̨����
int station_num;
//����̨λ����Ϣ��
StationInfo station_info_table[STATION_MAX_NUM];

//����һ������̨��Ϣ
void addStationInfo(int type, float x, float y){
    station_info_table[station_num].type = type;
    station_info_table[station_num].x = x;
    station_info_table[station_num++].y = y;
}

//����һ������̨��Ϣ
void updateStationInfo(int id, int time, int raw, int ok){
    station_info_table[id].time = time;
    station_info_table[id].raw = raw;
    station_info_table[id].ok = ok;
}

//������������̨��ֱ�߾���
float cacuStationDis(int id1, int id2){
    float disx = station_info_table[id1].x-station_info_table[id2].x;
    float disy = station_info_table[id2].y-station_info_table[id2].y;
    return sqrt(disx*disx+disy*disy);
}

//����̨����ϵ��
StationTaskRelation station_task_table[STATION_MAX_NUM][STATION_MAX_NUM];

//���i->j��������Ϊtask_id
void addStationTask(int s,int d,int task_id){
    station_task_table[s][d].task_id = task_id;
}

//��ȡ����ʱ��
int getTimeOfStation(int id){
	return station_info_table[id].time;
}

//��ȡ��Ʒ��״̬
int getOkOfStation(int id){
	return station_info_table[id].ok;
}

//��ȡԭ���ϸ�״̬
int getRawOfStation(int id){
	return station_info_table[id].raw;
}

//��ȡ����̨״̬
int getTypeOfStation(int id){
	return station_info_table[id].type;
}