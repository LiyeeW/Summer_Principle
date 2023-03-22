#ifndef move_conf_h
#define move_conf_h

#include "Robot.h"
#include "Movement.h"
#include "MoveTrace.h"

using namespace std;


//stage3机器人遇到其他stage机器人距离多远时，需要开始避障，单位m
const float MIN_SAFE_DISTANCE = 2 * ROBOT_RADIUS + 2 * SQRT_THREE;

//对窗口距离的裕度放宽倍数
const float WINDOWS_MARGIN = 1.5;

//夹角在多少以内算平，包括包括相向和同向，平的夹角不能用交点窗口法避障
const float FLAT_ANGLE = PI/6;


//机器人两两之间的冲突信息记录，因为[m][n](m>=n)不用，故只使用小一半的容量
typedef struct {
    int type;       //冲突状态机的类型，-1表示初始状态（0保留，可能吞并原基础运动系统）
    int stage;      //冲突状态机的阶段
    float distance; //两者圆心直线距离
    bool across;    //两者的轨迹线段是否有交点
    float x;        //若有交点，交点的x坐标
    float y;        //若有交点，交点的y坐标
    float orient;   //若有交点，两者轨迹的夹角（TODO:暂时没想好正负或取绝对值）
    float winDist;  //若有交点，且夹角不算平，该值表示窗口距离的长度，单位m；临界点距离交点半个windows；实际返回时会再乘一个MARGIN
    int winFrame;   //若有交点，且夹角不算平，该值表示全速度过窗口的所需帧数
    int comeFrame1; //前者机器人到达窗口临界点所需的帧数
    int comeFrame2; //后者机器人到达窗口临界点所需的帧数
    int richFrame1; //前者机器人的目标工作台留给它的充裕帧数；如果任务紧凑，该值一般为0
    int richFrame2; //后者机器人的目标工作台留给它的充裕帧数；如果任务紧凑，该值一般为0
    int whoWait;    //如果在执行窗口避障，提前停车避开窗口冲突的机器人编号，必须是m和n之间的一个
} RobotConf;



//机器人之间的冲突信息表
extern RobotConf robot_conf_table[ROBOT_NUM][ROBOT_NUM];  


//避障系统必要的全局初始化工作
void initMoveConfGlobal();

//在每一帧运动执行前，更新全局的冲突信息，包含信息和冲突状态机的更新
void updateMoveConf();

//(每一帧)检查并执行机器人的可能的冲突解决，返回false时，调用者会继续执行基础的运动系统
bool checkMoveConf(int robot_id);

#endif