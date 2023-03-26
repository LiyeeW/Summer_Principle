#ifndef move_conf_h
#define move_conf_h

#include "Robot.h"
#include "Movement.h"
#include "MoveTrace.h"

using namespace std;

//相向夹角在多少以内算平
const float FLAT_ANGLE = PI/6;

//对窗口距离的裕度放宽倍数
const float WINDOWS_MARGIN = 1.2;


//已定义的冲突类型数量，包括无冲突
const int CONF_TYPE_NUM = 3;

//最大阶段数
const int MAX_STAGE_NUM = 5;

//角色数
const int ROLE_NUM = 2;

//静止参考表
extern bool STAGE_STILL[CONF_TYPE_NUM][MAX_STAGE_NUM][ROLE_NUM];



//stage3机器人遇到其他stage机器人距离多远时，需要开始避障，单位m
const float MIN_SAFE_DISTANCE = 2 * ROBOT_RADIUS + 2 * SQRT_THREE;






//旋转时的固定角速度
const float ROTATE_OMEGA = 1.6;

//旋转时的最小、最大线速度
const float ROTATE_LOW_SPEED = 1;
const float ROTATE_UP_SPEED = 6;

//机器人两两之间的冲突信息记录，因为[m][n](m>=n)不用，故只使用小一半的容量
typedef struct {
    int type;       //冲突状态机的类型：0-无冲突，1-平相向，2-等待
    float assess;   //紧急程度评估
    int stage;      //冲突状态机的阶段
    float distance; //两者圆心直线距离
    bool across;    //两者的轨迹线段是否有交点
    float x;        //若有交点，交点的x坐标
    float y;        //若有交点，交点的y坐标
    float orient;   //若有交点，两者轨迹的夹角（TODO:暂时没想好正负或取绝对值）
    float winDist;  //若有交点，且夹角不算平，该值表示窗口距离的长度，单位m；临界点距离交点半个windows；实际使用时会再乘一个MARGIN
    int winFrame;   //若有交点，且夹角不算平，该值表示全速度过窗口的所需帧数
    int comeFrame1; //前者机器人到达窗口临界点所需的帧数
    int comeFrame2; //后者机器人到达窗口临界点所需的帧数
    int richFrame1; //前者机器人的目标工作台留给它的充裕帧数；如果任务紧凑，该值一般为0
    int richFrame2; //后者机器人的目标工作台留给它的充裕帧数；如果任务紧凑，该值一般为0
    int role[ROLE_NUM];      //0-1号角色对应的机器人编号，TODO:role需要在识别时暂时分配，便于冲突集查找对应的机器人
} RobotConf;


//机器人自身的冲突解决信息记录
// typedef struct {    
//     bool solving;   //是否正在解决冲突
//     int partner;    //冲突对方的机器人编号
//     int role;       //冲突处理下的角色，0或1
//     // TODO bool still;     //是否静止，由所处的type-stage-role决定，并不是看当前速度决定
// } RobotSolve;



//机器人之间的冲突信息表
extern RobotConf robot_conf_table[ROBOT_NUM][ROBOT_NUM];  

//获取两个机器人之间的冲突对
RobotConf* getConfPair(int a, int b);

//获取机器人正在解决的冲突
RobotConf* getConfSolving(int robot_id);

//获取机器人正在解决的冲突，返回值不能为nullptr
RobotConf* getConfSolving(int robot_id, bool must);

//设置机器人正在解决的冲突
void setConfSolving(int robot_id, RobotConf* confp);

//获取机器人是否静止
bool getStageStill(int robot_id);

//获取机器人解决冲突的角色
int getConfRole(int robot_id);

// //获取机器人解决冲突的对象
// int getConfSolvePartner(int robot_id);

// //设置机器人解决冲突的对象
// void setConfSolvePartner(int robot_id, int partner);


//读取冲突类型
int getConfType(int robot_id);

//读取冲突阶段
int getConfStage(int robot_id);

//获取冲突角色
int getConfRole(int robot_id);

//获取机器人是否静止
bool getStageStill(int robot_id);

//设置冲突类型
void setConfType(RobotConf* confp, int type);

//读取冲突阶段
int getConfStage(RobotConf* confp);

//设置冲突阶段
void setConfStage(RobotConf* confp, int stage);

//设置冲突评估值
void setConfAssess(RobotConf* confp, float assess);

//执行-冲突-冲突对中，游戏开始时，全局的初始化工作
void initConfpairGameStart();

//在每一帧运动执行前，更新全局的冲突信息，包含信息和冲突状态机的更新
void updateMoveConf();

//是否角度算平
bool getPairFlat(RobotConf* confp);

//是否方向相向
bool getPairOppo(RobotConf* confp);

//获取双方到交点的距离之和
float getPairAcrossDistSum(RobotConf* confp);

//role[0], role[1]是两个不同的机器人的编号
//robot_id参数一定是其中一个；函数功能是设定robot_id为新的role[0]，剩余的那个机器人编号为新的role[1]
void setConfRole(RobotConf* confp, int robot_id);

//返回在夹角中线的偏逆时针方向的一方的机器人编号
int getPairAntiOne(RobotConf* confp);

//获得窗口长度
float getPairWinDist(RobotConf* confp);

//获取其中一方到交点的距离
float getPairAcrossDist(RobotConf* confp, int robot_id);

#endif