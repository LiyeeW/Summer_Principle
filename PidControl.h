#ifndef pid_control_h
#define pid_control_h

//对某个变量的PID控制的所需信息
 typedef struct {
    float offset;  //偏移值，即目标值-实际值
    float sum_offset;   //累计的偏移值
    float dif_offset;   //本轮与上一轮的偏移值的差
    float kp;        //比例系数p
    float ki;        //积分系数i
    float kd;        //微分系数d
    float up_limit;  //输出的上限
    float low_limit;    //输出的下限
 } PidControl;


//初始化一组PID控制
void initPidControlTable(int num, PidControl* pc, float kp, float ki, float kd, float up_limit, float low_limit);

//重置一个PID控制
void resetPidControl(int id, PidControl* pc);

//启动一个PID控制的一轮迭代，返回PID控制的输出值
float launchPidControl(int id, PidControl* pc, float offset);


#endif 