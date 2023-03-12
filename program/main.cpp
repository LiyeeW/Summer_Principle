// main.cpp: 定义应用程序的入口点。
//

#include "CheckerIO.h"
#include "Decision.h"

using namespace std;

int main()
{
	readMap();
	//TODO
	sendOK();
	while(readFrame()){
		makeDecision();
		writeDecision();
	}
	return 0;
}
