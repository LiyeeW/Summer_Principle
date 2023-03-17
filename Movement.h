#ifndef movement_h
#define movement_h

#include "PidControl.h"
#include "Robot.h"



/* 用于调整单个机器人的角速度和速度
 * 1.0版本的策略：
 * 1）方向相差在正负15度之内，并且距离相差0.02s * 6m/s * 5 =0.6m ，则调整角速度为0，速度6m全速前进
 * 2）不满足方向，则调整速度为0，角速度为2全速满足方向
 * 3）方向满足，距离相差为0.6m，
*/





#endif