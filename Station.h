#ifndef station_h
#define station_h

#include <math.h>

//�����̨����
const int STATION_MAX_NUM = 50;
//����̨����
extern int station_num;

//����̨�ı��ھ������Ĺؼ���Ϣ
typedef struct {
	int type;	//1-9�Ĺ���̨����
	float x;	//x����
	float y;	//y����
	int time;	//ʣ������ʱ��
	int raw;	//ԭ���ϸ�״̬
	int ok;		//��Ʒ��״̬
} StationInfo;


//����̨λ����Ϣ��
extern StationInfo station_info_table[STATION_MAX_NUM];

//����һ������̨��Ϣ
void addStationInfo(int type, float x, float y);

//����һ������̨��Ϣ
void updateStationInfo(int id, int time, int raw, int ok);

//������������̨��ֱ�߾���
float cacuStationDis(int id1, int id2);

//����̨֮���task_id���Ա�ʾ����������
typedef struct{
	int task_id;
}StationTaskRelation;

//����̨����ϵ��
extern StationTaskRelation station_task_table[STATION_MAX_NUM][STATION_MAX_NUM];

//���i->j��������Ϊtask_id
void addStationTask(int s,int d,int task_id);

//��ȡ����ʱ��
int getTimeOfStation(int id);

//��ȡ��Ʒ��״̬
int getOkOfStation(int id);
//��ȡԭ���ϸ�״̬
int getRawOfStation(int id);

//��ȡ����̨״̬
int getTypeOfStation(int id);


#endif