#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"
#include "task.h"
#include "Bid.h"

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

    //������ֿ��л����ˣ�
    //1. ������������bids������ɵ�task����bid��
    //2. ������������л����ˣ����������ܳ�ʱ��
    //��bidsѡ��ǰ���л����˱�����ߵ�������з��䣬ѡ��task�����Ϊæµ����task��Ӧ����bidʧЧ

    //��ʼ���ԣ�
    for(int i=0;i<robot_num;i++){
        for(BidInfo bid: bids_list){
            if(bid.robot_id == i){  //�����i�Ż�����
                addDecision(0,i,6);
                break;
            }
        }
    }
    //end

    //û�г��ֿ��л����ˣ��Ȳ���

    //addDecision(0,0,6);
}

//���ѡ��
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}

//��ͼ��ʼ���������������� �Լ� ���б��۲����� �򵥵ط�������
//��ʼ������е�Ȩ�� = ʣ������ʱ��+���/���صľ���
void initMap(void){
    //generate all tasks:
    for(int i=0;i<station_num;i++){
        for(int j=0;j<station_num;j++){
            switch (station_info_table[i].type)
            {
            case 1:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 2:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==6 || station_info_table[j].type==9){
                    int value=3200;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 3:
            {
                if(station_info_table[j].type==6 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3400;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 4:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7100;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 5:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7800;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 6:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=8300;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            case 7:
            {
                if(station_info_table[j].type==8 || station_info_table[j].type==9){
                    int value=29000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    //generate all bids:
    for(int i=0;i<robot_num;i++){
        for(int j=0;j<task_num;j++){
            float xx = robot_info_table[i].x-station_info_table[waiting_task_list[j].source].x;
            float yy = robot_info_table[i].y-station_info_table[waiting_task_list[j].source].y;
            addBidInfo(i,j,sqrt(xx+yy)); 
            //������Ҫ��һ����ȷ�� TODO
        }
    }
    sortBidList();
    //�򵥵ط�������
}
