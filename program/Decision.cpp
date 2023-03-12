#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"

#include <cstring>
#include <iostream>

using namespace std;

//选择的数量
int decision_num = 0;


//选择类型对应的字符串
char decision_string[DECISION_TYPE_NUM][10] = {"forward", "rotate", "buy", "sell", "destroy"};
//选择信息表
DecisionInfo decision_info_table[50];


//新增选择
void addDecision(int type, int robot, float param){
    decision_info_table[decision_num++] = {type, robot, param};
}

//做出选择
void makeDecision(){
    //清空上一轮的选择
    decision_num = 0;
    //TODO
    addDecision(0,0,6);
}

//输出选择
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}