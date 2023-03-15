// main.cpp: 定义应用程序的入口点。
//

#include "CheckerIO.h"
#include "Decision.h"

using namespace std;

int main()
{
	setDDL(5*1000);
	readMap();
	//简单的任务队列初始化
	initMap();
	waitDDL();
	sendOK();
	while(readFrameHead()){
		setDDL(15);
		readFrameAll();
		makeDecision();
		writeDecision();
		waitDDL();
		sendOK();
	}
	return 0;
}
