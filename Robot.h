#ifndef robot_h
#define robot_h

//机器人数量
const int ROBOT_NUM = 4;
extern int robot_num;

//机器人基本信息
typedef struct {
	int station;	//所在的工作台ID
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

	//增加机器人的当前任务执行状态，这两个变量用于运动系统和决策系统的通信交互
	int task_id;   //分配的任务id
	int task_status;    //-1：没有任务  0：正在前往取货点   1：已经取货了，正在前往收货点	 2：已完成	

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

#endif