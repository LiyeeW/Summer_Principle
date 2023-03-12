#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"

#include <cstring>
#include <iostream>

using namespace std;

//ѡ�������
int decision_num = 0;


//ѡ�����Ͷ�Ӧ���ַ���
char decision_string[DECISION_TYPE_NUM][10] = {"forward", "rotate", "buy", "sell", "destroy"};
//ѡ����Ϣ��
DecisionInfo decision_info_table[50];


//����ѡ��
void addDecision(int type, int robot, float param){
    decision_info_table[decision_num++] = {type, robot, param};
}

//����ѡ��
void makeDecision(){
    //�����һ�ֵ�ѡ��
    decision_num = 0;
    //TODO
    addDecision(0,0,6);
}

//���ѡ��
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}