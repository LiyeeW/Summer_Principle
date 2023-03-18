#include "Movement.h"
#include "Robot.h"
#include "Station.h"
#include "task.h"
#include <cmath>
#include <iostream>

//(TODO)首先，在地图初始化阶段，需要调用initPid，设定PID参数
//(OK)之后，在每次切换目标地点时，需要调用resetPID
//(TODO)在获取到每一帧数据后，需要调用launchPID
//(TODO)PID的参数需要结合判题器做实验，根据经验人为设置
//(TODO)需要先设定速度，再设定角速度，因为当速度为负时，实际朝向会加减一个PI

//记录机器人当前的运动阶段：
//1表示角速度极小、朝向准确的刹车等待，需要直线紧急减速，甚至倒退，只有距离PID
//2表示角速度较大、朝向不准确的刹车等待，需要紧急减速到0，再原地调整方向，再直线移动，先角度PID，再距离PID
//3表示全速锁定方向，只有角度PID
int movement_edge_table[ROBOT_NUM];

//机器人到目的地的直线路径角度表
float direct_orient_table[ROBOT_NUM];
//机器人到目的地的直线距离表，在特定的减速接近阶段，会结合朝向判定距离的正负
float direct_distance_table[ROBOT_NUM]; 
//机器人如果正在取货，源材料是否已准备好；如果是送货，目前方案不需要等待
bool need_wait_table[ROBOT_NUM];

//(TODO)每个机器人的当前目标工作台，每次切换目标地点时更新
int dest_station[ROBOT_NUM];


//用PID算法控制机器人锁定方向的必要信息
PidControl orient_pid_table[ROBOT_NUM];
//用PID算法控制直线行驶的必要信息
PidControl distance_pid_table[ROBOT_NUM];

//重置直线朝向的记录表，使得初始直线偏角记录全部大于PI，便于后续通过新旧差值突变判断“已越过”
void resetDirectOrient(int robot_id){
    direct_orient_table[robot_id] = 2 * PI;
}
void resetDirectOrientTable(){
    for(int i=0;i<ROBOT_NUM;i++){
        resetDirectOrient(i);
    }
}

//更新运动阶段
void updateMoveEdge(int robot_id){
    //
}



//获取机器人的目标工作台
int getRobotDest(int robot_id){
    return dest_station[robot_id];
}

//(TODO调用时机)切换更新某机器人的目标工作台，此时task_status只可能是0或1
void updateRobotDest(int robot_id){
    //重置PID控制
    resetPidControl(robot_id, orient_pid_table);
    //TODO：两种控制的开始与结束周期不一定是按目的地切换为划分
    resetPidControl(robot_id, distance_pid_table);
    //重置朝向记录
    resetDirectOrient(robot_id);
    int task_id = getTaskofRobot(robot_id);
    int task_status = robot_info_table[robot_id].task_status;
    // 如果正在取货，目的地是task的source
    if(task_status == 0) dest_station[robot_id] = getSourceOfTask(task_id);
    // 如果正在送货，目的地是task的dest
    else if(task_status == 1) dest_station[robot_id] = getDestOfTask(task_id);
    // 如果都不是，则报错
    else cerr<<"movement: robot "<<robot_id<<" task status "<<task_status<<endl;
}



//(每一帧)更新某机器人的到目的地的直线距离和朝向
void updateRobotDestDirect(int robot_id){
    int station_id = getRobotDest(robot_id);
    float dy = station_info_table[station_id].y - robot_info_table[robot_id].y;
    float dx = station_info_table[station_id].x - robot_info_table[robot_id].x;
    direct_distance_table[robot_id] = sqrt(dy*dy+dx*dx);
    float orient = (float)atan(dy/dx);
    //如果不是第一次记录朝向，并且处于直线紧急减速阶段
    if(direct_orient_table[robot_id] < 1.5*PI && movement_edge_table[robot_id] == 1){
        //计算朝向有无突变
        float orient_diff = abs(orient - direct_orient_table[robot_id]);
        //考虑浮点计算精度，若突变量为PI，则距离为负
        if(orient_diff > 0.9 * PI) direct_distance_table[robot_id] = -direct_distance_table[robot_id];
    }
    direct_orient_table[robot_id] = orient;
}


//(每一帧)更新机器人是否需要在临近时降速，等待生产
void updateNeedWait(){
    for(int i=0;i<ROBOT_NUM;i++){
        int station_id = getRobotDest(i);
        // 机器人正在送货，或取货的源材料已准备好
        need_wait_table[i] = (robot_info_table[i].task_status == 0) && (station_info_table[station_id].ok == 0);
    }
}



//初始化角速度的pid(目前带参数，为方便做实验找PID参数)
void orientInitPid(float kp, float ki, float kd){
    initPidControlTable(ROBOT_NUM, orient_pid_table, kp, ki, kd, ORIENT_LIMIT, -ORIENT_LIMIT);
}

//进行一轮PID控制，返回角速度控制值
float orientLaunchPid(int robot_id){
    //计算在当前位置的目标角度
    float dest = direct_orient_table[robot_id];
    //根据帧信息获取当前实际角度
    float real = robot_info_table[robot_id].orient;
    //获取Pid输出
    float output = launchPidControl(robot_id, orient_pid_table, dest-real);
    return output;
}


//初始化直线距离的PID控制
void distanceInitPid(float kp, float ki, float kd){
    initPidControlTable(ROBOT_NUM, distance_pid_table, kp, ki, kd, DIRECT_UP_LIMIT, DIRECT_LOW_LIMIT);
}

//进行一轮PID控制，返回直线速度控制值
float distanceLaunchPid(int robot_id){
    //计算在当前位置的目标角度
    float dist = direct_distance_table[robot_id];
    //获取Pid输出
    float output = launchPidControl(robot_id, orient_pid_table, dist);
    return output;
}

//必要的全局初始化工作
void initMoveGlobal(){
    resetDirectOrientTable();
    orientInitPid(0.5, 0.1, 0.05);
    distanceInitPid(0.5, 0.1, 0.05);
}