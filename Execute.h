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

//（在决策确定后调用，不论并行还是串行）制订本帧的执行方案
void makeExecute();
//输出执行方案
void outputExecute();

#endif
