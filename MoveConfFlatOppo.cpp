#include "MoveConfFlatOppo.h"
#include "MoveTrace.h"
#include "MoveConf.h"

namespace MoveConfFlatOppo{

//TODO: 初始化
void init(int a, int b){

}

//识别函数
//TODO:role需要在识别时暂时分配，便于冲突集查找对应的机器人
void recognize(int a, int b){
    //如果有交点
    //如果角度算平
    //如果角度相向
    //两人之和：临界(有乘上裕度因子)到交点的长度/当前位置到交点的长度
}

//重置函数，在确定开始解决该冲突时的重置工作
void reset(int a, int b){
    //确定同时往哪个方向转
    //计算旋转时的外轮廓圆弧能否拉满，确定旋转时的线速度
    //本类冲突对称处理，不需要区分角色
}

//该冲突的状态机切换函数
void launch(int a, int b){
    //stage 1：如果两者轮廓带没有相交，或者两者夹角达到max，则退出（大概率回到常规的stage 2）
}

//该冲突的状态机执行函数
void execute(int a, int b){
    //stage 1:匀速圆周运动
}



};