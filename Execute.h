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

//制订本帧的销售执行方案
void executeTrade();
//制订本帧的运动执行方案
void executeMove();
//输出执行方案
void outputExecute();

//执行系统的初始化工作
void initExecuteGlobal();

#endif
