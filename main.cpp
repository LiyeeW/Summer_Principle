// main.cpp: 定义应用程序的入口点。
//

#include "CheckerIO.h"
#include "Decision.h"
#include "Execute.h"

using namespace std;

int main()
{
	//setDDL(4970);
	readMap();
	//简单的任务队列初始化
	initMap();
	//waitDDL();
	sendOK();
	while(readFrameHead()){
		//20-30:6000; 15:4800
		setDDL(35);
		readFrameAll();
		makeDecision();  //
		makeExecute();
		writeExecute();
		//waitDDL();
		sendOK();
	}
	return 0;
}
