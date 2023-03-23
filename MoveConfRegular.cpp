#include "MoveConf.h"
#include "MoveConfRegular.h"



namespace MoveConfRegular{



//TODO: 初始化，参数是个摆设
void init(int a, int b){

}

//识别：0表示不冲突
void recognize(int a, int b){
    setConfType(a,b,LOCAL_TYPE);
    //评估：恒为不紧急
    setConfAssess(a,b,0);
}

//重置函数，重置该状态机
void reset(int a, int b){
    //type设为0，初始stage为2，不区分role
    //重置PID控制
}


//该冲突的状态机切换函数
void launch(int a, int b){
    //stage 2: 到1或3
}


//该冲突的状态机执行函数
void execute(int a, int b){

}




}