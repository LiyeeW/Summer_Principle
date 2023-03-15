#ifndef checker_io_h
#define checker_io_h

using namespace std;
const int DOT_NUM = 100;


//发送一行ok
void sendOK();
//读取输入地图
bool readMap();
//读输入的当前帧的头部
bool readFrameHead();
//读取输入的当前帧的剩余完整信息
void readFrameAll();
//输出选择信息
void writeDecision();
//设定ddl
void setDDL(int ms);
//检查睡到ddl
void waitDDL();







#endif