#ifndef robot_h
#define robot_h



const float PI = 3.14159f;
// 角速度上下限的绝对值
const float ORIENT_LIMIT = PI;
//线速度的上下限
const float SPEED_UP_LIMIT = 6;
const float SPEED_LOW_LIMIT = -2;

//机器人数量
const int ROBOT_NUM = 4;
extern int robot_num;

//机器人基本信息
typedef struct {
	int station;	//所在工作台ID
	int item;		//携带的物品
	int time;		//持有帧数
	int collision;	//碰撞次数
	float timeValue;//时间价值系数
	float collisionValue;	//碰撞价值系数
	float omega;	//角速度
	float xSpeed;	//x方向速度
	float ySpeed;	//y方向速度
	float orient;	//朝向
	float x;	//x坐标
	float y;	//y坐标

	//增加机器人当前任务执行状态，用于决策系统和运动系统的交互通信
	int task_id;   //分配的任务id
	int task_status;    //-1：未分配任务  0：正在前往取货   1：已取货，正在送货	 2：送货完毕	

} RobotInfo;

extern RobotInfo robot_info_table[ROBOT_NUM];

//新增一个机器人记录
void addRobotInfo(float x, float y);

//更新一个机器人记录
void updateRobotInfo(int id, int s, int i, float t, float c, float o, float xS, float yS, float ori, float x, float y);

//为机器人分配任务
void addTasktoRobot(int robot_id, int id, int status);

//获取当前分配的任务id
int getTaskofRobot(int id);

//获取当前任务状态
int getTaskStatusofRobot(int id);

//修改当前任务状态
void setTaskStatusofRobot(int id, int status);

//获取机器人x坐标
float getRobotAxisX(int robot_id);

//获取机器人y坐标
float getRobotAxisY(int robot_id);

#endif