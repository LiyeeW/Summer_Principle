
#include "CheckerIO.h"
#include "Station.h"
#include "Robot.h"
#include "Score.h"
#include "Decision.h"

#include <iostream>

using namespace std;

//在读取输入时的处理函数指针
typedef void (*FuncPtr)(char* line, int seq);

//发送一行ok
void sendOK(){
    puts("OK");
    fflush(stdout);
}

//读入一行原始地图后的工作站和机器人的位置记录
void getMapByLine(char* line) {
    //随行递减的y坐标
    static float y =49.75;
    for (int i = 0; i < DOT_NUM; i++) {
        if (line[i] == '.') continue;
        float x = 0.25f + i * 0.5f;
        if (line[i] == 'A') addRobotInfo(x, y);
        else addStationInfo(line[i] - '0', x, y);
    }
    y -= 0.5;
}

//读取输入地图
bool readMap() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            sendOK();
            return true;
        }
        getMapByLine(line);
    }
    return false;
}



//读取输入的当前帧
bool readFrame() {
    if (scanf("%d", &current_frame) == EOF) return false;
    scanf("%d%*d", &current_score);
    //更新工作台信息
    for (int i = 0; i < station_num; i++) {
        int a, b, c;
        scanf("%*d%*f%*f%d%d%d", &a, &b, &c);
        updateStationInfo(i, a, b, c);
    }
    //更新机器人信息
    for (int i = 0; i < robot_num; i++) {
        int station, item;
        float timeValue, collisionValue, omega, xSpeed, ySpeed, orient, x, y;
        scanf("%d%d%f%f%f%f%f%f%f%f", &station, &item, &timeValue, &collisionValue, &omega, &xSpeed, &ySpeed, &orient, &x, &y);
        updateRobotInfo(i, station, item, timeValue, collisionValue, omega, xSpeed, ySpeed, orient, x, y);
    }
    getchar();
    char line[1024];
    fgets(line, sizeof line, stdin);
    sendOK();
    return true;
}

//输出选择信息
void writeDecision(){
    printf("%d\n", current_frame);
    outputDecision();
    sendOK();
}
