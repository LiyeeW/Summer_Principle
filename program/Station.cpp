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
