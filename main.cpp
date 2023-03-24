// main.cpp: 定义应用程序的入口点。
//

#include "CheckerIO.h"
#include "MoveStage.h"
#include "Movement.h"
#include "Decision.h"
#include "Execute.h"
#include "Score.h"
#include<iostream>

using namespace std;

int main()
{
	//setDDL(4970);
	readMap();
	//简单的任务队列初始化
	initMap();
	//执行系统初始化，包括执行、基础运动和避障运动
	initExecuteGlobal();
	//waitDDL();
	sendOK();
	while(readFrameHead()){
		//20-30:6000; 15:4800
		setDDL(35);
		readFrameAll();
		//更新这帧的运动系统数据
		updateMovePerFrame();
		//制订本帧的交易执行方案
		executeTrade(); 
		//决策
		makeDecision(); 
		//制订本帧的运动执行方案
		executeMove();
		writeExecute();
		//waitDDL();
		sendOK();
	}
	return 0;
}
