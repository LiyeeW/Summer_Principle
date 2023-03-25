#ifndef execute_h
#define execute_h

//执行类型的数量
const int EXECUTE_TYPE_NUM = 5;



//执行信息
typedef struct {
    int type;       //0-4的执行类型
    int robot;      //机器人ID
    float param;    //参数2
} ExecuteInfo;

//执行系统中，游戏开始前，全局需要做的初始化工作
void initExecuteGameStart();
//执行系统中，拿到每帧传感器数据后，全局需要做的更新工作
void updateExecutePerFrame();
//制订本帧的销售执行方案
void executeTrade();
//制订本帧的运动执行方案
void executeMove();
//输出执行方案
void outputExecute();


#endif
