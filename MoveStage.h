#ifndef move_stage_h
#define move_stage_h

//运动系统必要的全局初始化工作
void initMoveGlobal();

//每帧更新运动记录，包括直线角度与距离、是否需要等待、运动阶段等
void updateMovePerFrame();

//根据运动阶段和PID算法得到下一步的角速度和线速度
void moveByStage(int robot_id);

//在某机器人朝新的目的地出发前，对运动系统的重置工作
void resetMoveBeforeDepart(int robot_id);


#endif