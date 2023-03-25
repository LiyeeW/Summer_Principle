#include "Robot.h"
#include <iostream>
#include<math.h>

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

//获取当前任务状态
int getTaskStatusofRobot(int id){
	return robot_info_table[id].task_status;
}

//修改当前任务状态
void setTaskStatusofRobot(int id, int status){
	robot_info_table[id].task_status = status;
}

//获取机器人x坐标
float getRobotAxisX(int robot_id){
	return robot_info_table[robot_id].x;
}

//获取机器人y坐标
float getRobotAxisY(int robot_id){
	return robot_info_table[robot_id].y;
}


//获取机器人实际线速度
float getRobotSpeed(int robot_id){
	float vy = robot_info_table[robot_id].ySpeed, vx = robot_info_table[robot_id].xSpeed;
	float v = sqrt(vy*vy+vx*vx);
	//假设垂直
    float orient = 0.5*PI;
    //如果垂直且dy为负
    if(abs(vx) < 0.001 && vy<0) orient = -orient;
    else{
        //算得[-0.5PI,0.5PI]范围的角度
        orient = (float)atan(vy/vx);
        //根据非0的dx扩大到[-PI,PI]范围的角度
        if(vx<0) orient+=(vy<0)?-PI:PI;
    }
	if(abs(orient-robot_info_table[robot_id].orient)>0.9*PI) return -v;
	return v;
}