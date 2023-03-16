#include "Robot.h"
#include <iostream>

using namespace std;

//��������Ϣ��
RobotInfo robot_info_table[ROBOT_NUM];

//����������
int robot_num = 0;

//����һ�������˼�¼
void addRobotInfo(float x, float y){
	robot_info_table[robot_num].x = x;
	robot_info_table[robot_num].task_id = -1;
	robot_info_table[robot_num].task_status = -1;
	robot_info_table[robot_num++].y = y;
	//cerr<<"new robot "<<x<<" "<<y<<endl;
}

//����һ�������˼�¼
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y){
	int time = robot_info_table[id].time;
	int collision = robot_info_table[id].collision;
	robot_info_table[id] = {s, i, time, collision, t, c, o, xS, yS, ori, x, y};
	//cerr<<"update robot"<<x<<" "<<y<<endl;
}

//Ϊ�����˷�������
void addTasktoRobot(int robot_id, int id, int status){
	robot_info_table[robot_id].task_id = id;
	robot_info_table[robot_id].task_status = status;
}

//��ȡ��ǰ���������id
int getTaskofRobot(int id){
	return robot_info_table[id].task_id;
}