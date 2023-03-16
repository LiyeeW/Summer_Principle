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

	//���ӻ����˵ĵ�ǰ����ִ��״̬�����������������˶�ϵͳ�;���ϵͳ��ͨ�Ž���
	int task_id;   //���������id
	int task_status;    //-1��û������  0������ǰ��ȡ����   1���Ѿ�ȡ���ˣ�����ǰ���ջ���	 2�������	

} RobotInfo;

extern RobotInfo robot_info_table[ROBOT_NUM];

//����һ�������˼�¼
void addRobotInfo(float x, float y);

//����һ�������˼�¼
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y);

//Ϊ�����˷�������
void addTasktoRobot(int robot_id, int id, int status);

//��ȡ��ǰ���������id
int getTaskofRobot(int id);

#endif