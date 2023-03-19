#include "Robot.h"
#include <iostream>

using namespace std;

//机器人信息表
RobotInfo robot_info_table[ROBOT_NUM];

//机器人数量
int robot_num = 0;

//新增一个机器人记录
void addRobotInfo(float x, float y){
	robot_info_table[robot_num].x = x;
	robot_info_table[robot_num].task_id = -1;
	robot_info_table[robot_num].task_status = -1;
	robot_info_table[robot_num++].y = y;
	//cerr<<"new robot "<<x<<" "<<y<<" "<<robot_info_table[robot_num-1].task_status<<endl;
}

//更新一个机器人记录
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y){
	int time = robot_info_table[id].time;
	int collision = robot_info_table[id].collision;
	int task_id = robot_info_table[id].task_id;
	int task_status = robot_info_table[id].task_status;
	robot_info_table[id] = {s, i, time, collision, t, c, o, xS, yS, ori, x, y,task_id,task_status};
	//cerr<<"update robot"<<x<<" "<<y<<endl;
}

//为机器人分配任务
void addTasktoRobot(int robot_id, int id, int status){
	robot_info_table[robot_id].task_id = id;
	robot_info_table[robot_id].task_status = status;
}

//获取当前分配的任务id
int getTaskofRobot(int id){
	return robot_info_table[id].task_id;
}