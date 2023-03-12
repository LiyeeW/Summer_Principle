#ifndef robot_h
#define robot_h

//����������
const int ROBOT_NUM = 4;
extern int robot_num;

//�����˻�����Ϣ
typedef struct {
	int station;	//���ڵĹ���̨ID
	int item;		//Я������Ʒ
	int time;		//����֡��
	int collision;	//��ײ����
	float timeValue;//ʱ���ֵϵ��
	float collisionValue;	//��ײ��ֵϵ��
	float omega;	//���ٶ�
	float xSpeed;	//x�����ٶ�
	float ySpeed;	//y�����ٶ�
	float orient;	//����
	float x;	//x����
	float y;	//y����
} RobotInfo;

extern RobotInfo robot_info_table[ROBOT_NUM];

//����һ�������˼�¼
void addRobotInfo(float x, float y);

//����һ�������˼�¼
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y);

#endif