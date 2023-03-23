#ifndef move_conf_wait_h
#define move_conf_wait_h



namespace MoveConfWait { 




//本地冲突类型
const int LOCAL_TYPE = 2;

//本地stage容量（一般0空出来）
const int LOCAL_STAGE_NUM = 2;



//初始化
void init(int a, int b);


//识别函数
void recognize(int a, int b);

//重置函数，在确定开始解决该冲突时的重置工作
void reset(int a, int b);

//该冲突的状态机切换函数
void launch(int a, int b);

//该冲突的状态机执行函数
void execute(int a, int b);


}; 



#endif