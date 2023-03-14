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
	robot_info_table[robot_num++].y = y;
	//cerr<<"new robot "<<x<<" "<<y<<endl;
}

//更新一个机器人记录
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y){
	int time = robot_info_table[id].time;
	int collision = robot_info_table[id].collision;
	robot_info_table[id] = {s, i, time, collision, t, c, o, xS, yS, ori, x, y};
	//cerr<<"update robot"<<x<<" "<<y<<endl;
}