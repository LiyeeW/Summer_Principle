// main.cpp: 定义应用程序的入口点。
//

#include "CheckerIO.h"
#include "Decision.h"
#include<unistd.h>

using namespace std;

int main()
{
	//sleep(10);
	readMap();
	//简单的任务队列初始化
	initMap();
	sendOK();
	while(readFrame()){
		makeDecision();
		writeDecision();
	}
	return 0;
}
