#include "MoveTrace.h"
#include "Movement.h"
#include "Score.h"
#include <cmath>
#include <iostream>




//机器人轨迹信息表
RobotTrace robot_trace_table[ROBOT_NUM];

//在某机器人朝新的目的地出发前，对轨迹记录的重置工作
void resetTraceBeforeDepart(int robot_id){
    //更新终点坐标
    getAxis(getRobotDest(robot_id), robot_trace_table[robot_id].xd, robot_trace_table[robot_id].yd);
} 

//根据dy和dx计算k
float updateTraceLineK(int robot_id, float dy, float dx){
    //如果垂直，近似斜率为MAX_K
    if(dx==0) return MAX_K;
    float k = dy/dx;
    //接近垂直时，也近似为MAX_K
    if(abs(k)>MAX_K) return k/abs(k)*MAX_K;
    return k;
}

//根据起点坐标和k计算b
float updateTraceLineB(int robot_id){
    return robot_trace_table[robot_id].ys - robot_trace_table[robot_id].xs * robot_trace_table[robot_id].k;
}

//(每帧)更新线段信息,updateTraceLine改名为updateMovetracePerframe
void updateMovetracePerframe(int robot_id){
    //更新起点坐标
    getAxis(ROB(robot_id), robot_trace_table[robot_id].xs, robot_trace_table[robot_id].ys);
    //计算斜率和截距，终点减起点
    float dy = robot_trace_table[robot_id].yd - robot_trace_table[robot_id].ys;
    float dx = robot_trace_table[robot_id].xd - robot_trace_table[robot_id].xs;
    robot_trace_table[robot_id].k = updateTraceLineK(robot_id, dy, dx);
    robot_trace_table[robot_id].b = updateTraceLineB(robot_id);
}

//向量(x,y)
class my_vector{
    int x,y;
    int flag;   //flag=0，表示向量起始点是(xs,ys)，flag=1表示起始点是(xd,yd)
    //返回(x1,y1)指向(x2,y2)的向量
public:
    my_vector(int x1,int y1,int x2,int y2){
        x = x2 - x1;
        y = y2 - y1;
    }
    //用于共线判定的,排序坐标，生成的向量(x,y)保证和x夹角小于90°
    my_vector(int robot_id){
        if(robot_info_table[robot_id].xd > robot_info_table[robot_id].xs){
            flag = 1;
        }else if(robot_info_table[robot_id].xd == robot_info_table[robot_id].xs){
            if(robot_info_table[robot_id].yd > robot_info_table[robot_id].ys){
                flag = 1;
            }else{
                flag = 0;
            }
        }else{
            flag = 0;
        }
        if(flag){   //s指向d
            x = robot_info_table[robot_id].xd - robot_info_table[robot_id].xs;
            y = robot_info_table[robot_id].yd - robot_info_table[robot_id].ys;
        }else{  //d指向s
            x = robot_info_table[robot_id].xs - robot_info_table[robot_id].xd;
            y = robot_info_table[robot_id].ys - robot_info_table[robot_id].yd;
        }
    }
    int getflag(){return flag;}
}

//计算向量间的叉积
int multiply(my_vector v1, my_vector v2){
    return v1.x*v2.y - v2.x*v1.y;
}

//计算两个线段是否相交需要调用此函数两次
//向量生成的准则是，v1=id1的起点到id2的起点，v2=id1的起点到id2的终点，vm=id1的起点到id1的终点
bool is_part_cross(int robot_id1,int robot_id2){
    my_vector v1 = my_vector(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].ys,
                    robot_trace_table[robot_id2].xs,robot_trace_table[robot_id2].ys);
    my_vector v2 = my_vector(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].ys,
                    robot_trace_table[robot_id2].xd,robot_trace_table[robot_id2].yd);
    my_vector vm = my_vector(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].ys,
                    robot_trace_table[robot_id1].xd,robot_trace_table[robot_id1].yd);
    if(multiply(v1,vm) * multiply(v2,vm) <= 0){
        return true;
    }else{
        return false;
    }
}

//两个机器人的线段是否有交点
bool isTraceCross(int robot_id1, int robot_id2){
    my_vector vector_robot1 = my_vector(robot_id1); //获取机器人的线段向量，保证与x轴夹角小于90°
    my_vector vector_robot2 = my_vector(robot_id2);
    //平行
    if(multiply(vector_robot1,vector_robot2)==0){
        my_vector vd = my_vector(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].ys,
                    robot_trace_table[robot_id2].xs,robot_trace_table[robot_id2].ys);
        if(multiply(vector_robot1,vd) != 0){
        //不共线
            return false;
        }else{
        //共线
            if(max(robot_trace_table[robot_id1].ys,robot_trace_table[robot_id1].yd) < min(robot_trace_table[robot_id2].ys,robot_trace_table[robot_id2].yd)
            || min(robot_trace_table[robot_id1].ys,robot_trace_table[robot_id1].yd) > max(robot_trace_table[robot_id2].ys,robot_trace_table[robot_id2].yd)
            || max(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].xd) < min(robot_trace_table[robot_id2].xs,robot_trace_table[robot_id2].xd)
            || min(robot_trace_table[robot_id1].xs,robot_trace_table[robot_id1].xd) > max(robot_trace_table[robot_id2].xs,robot_trace_table[robot_id2].xd)
            ){
            //投影无重合，说明不相交
                return false;
            }else{
                //共线重合，则判断是否前往同一个方向
                //如果不是前往一个方向，则是相撞，返回true;如果是前往一个方向，返回false
                return getRobotDestOrient(robot_id1)!=getRobotDestOrient(robot_id2);
            }
        }
    //不平行
    }else{
        return is_part_cross(robot_id1,robot_id2) && is_part_cross(robot_id2,robot_id1);
    }
}

//计算两个机器人的交点坐标和夹角
void updateMeetingInfo(int robot_id1, int robot_id2, int &x,int &y,int &orident){
    if(robot_trace_table[robot_id1].k == robot_trace_table[robot_id2].k){
        //共线重合的情况
        orident = 0;
        x = (robot_trace_table[robot_id1].xd + robot_trace_table[robot_id2].xd)/2;
        y = (robot_trace_table[robot_id1].yd + robot_trace_table[robot_id2].yd)/2;
    }else{
        x = (robot_trace_table[robot_id2].b-robot_trace_table[robot_id1].b)/(robot_trace_table[robot_id1].k-robot_trace_table[robot_id2].k);
        y = robot_trace_table[robot_id1].k*k+robot_trace_table[robot_id1].b;
        float o1 = getRobotDestOrient(robot_id1), o2 = getRobotDestOrient(robot_id2);
        float delta_o;
        if(o2>o1)   delta_o = o2-o1;
        else delta_o = o1-o2;
        if(delta_o>PI)  delta_o = 2*PI-delta_o;
        orident = delta_o;
    }
}

//更新每帧的机器人轨迹信息表
void updateMovetracePerframe(){
    for(int i=0;i<ROBOT_NUM;i++){
        updateMovetracePerframe(i);
    }
}