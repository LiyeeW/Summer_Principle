#include "MoveConf.h"
#include "MoveConfWait.h"

namespace MoveConfWait { 



//初始化
void init(int a, int b){

}


//识别函数
void recognize(int a, int b){

}

//重置函数，在确定开始解决该冲突时的重置工作
//通过solving判断谁等谁
//上述有误，必须清楚等的谁
void reset(int a, int b);

//该冲突的状态机切换函数
void launch(int a, int b);

//该冲突的状态机执行函数
void execute(int a, int b);


}; 