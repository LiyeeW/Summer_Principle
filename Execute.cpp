#include "Execute.h"
#include "Station.h"
#include "Robot.h"
#include "Movement.h"
#include <cstring>
#include <iostream>

using namespace std;





//执行类型对应的字符串
char execute_string[EXECUTE_TYPE_NUM][10] = {"forward", "rotate", "buy", "sell", "destroy"};
//执行信息表
ExecuteInfo execute_info_table[50];
//执行的数量
int execute_num = 0;


//新增执行
void addExecute(int type, int robot, float param){
    execute_info_table[execute_num++] = {type, robot, param};
}

// 318方案：全速锁定方向
void fullSpeedLockOrient(int robot_id){
    //暂时拉满线速度
    addExecute(0,robot_id,6);
    //获取角速度
    float omega = orientLaunchPid(robot_id);
    addExecute(1,robot_id,omega);
}

//到达目的地后的处理函数
void destArrived(int robot_id, int station_id){
    // 如果正在取货
    if(robot_info_table[robot_id].task_status == 0){
        //检查工作台的产品格
        if(getOkOfStation(station_id)==1){
            //购买生产好的产品
            addExecute(2,robot_id,0);
            //切换任务状态为送货
            robot_info_table[robot_id].task_status = 1;
        }
        else{
            //机器人已到目的地，但还在生产中，于是紧急减速
        }
    }
}

//路过某工作台时的工作
void doSomethingWhenPassBy(int robot_id, int station_id){
    cerr<<"execute: robot "<<robot_id<<" pass by station "<<station_id<<endl;
}

//（在决策确定后调用，不论并行还是串行）制订本帧的执行方案
void makeExecute(){
    //清空上一轮的执行方案
    execute_num = 0;
    //更新运动记录
    
    for(int i=0;i<ROBOT_NUM;i++){
        int real_station = robot_info_table[i].station;
        //如果还没到达任何工作台
        //目前是根据传感器数据在到达时才判断，后续会优化为“距离多少时”判断，便于针对当前目的点状态和下一个目的的地点执行动作
        if(real_station == -1){
            fullSpeedLockOrient(i);
        }
        else{
            int dest_station = getRobotDest(i);
            //如果只是路过
            if(real_station != dest_station) doSomethingWhenPassBy(i,real_station);
            //如果已到达
            else destArrived(i,dest_station);
        }
        
    }

}



//输出执行方案
void outputExecute(){
    for(int i=0;i<execute_num;i++){
        printf("%s %d", execute_string[execute_info_table[i].type], execute_info_table[i].robot);
        if(execute_info_table[i].type<=1) printf(" %f\n", execute_info_table[i].param);
        else printf("\n");
    }
}
