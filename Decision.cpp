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
    //0. �������������ɣ��������ɵ�����Ϊfree
    //1. �����ܻ�ȡ������id�б�
    //2. Ϊ���л���������bids
    //3. Ϊ���л����˷���task
    
    //������ɵ�����״̬����
    for(int i=0;i<robot_num;i++){
        if(robot_info_table[i].task_status == 2){
            //���л�����robot_id�������󣬸�������״̬
            tagFreeTask(getTaskofRobot(i));
        }
    }
    bool flag = false;
    for(int i=0;i<robot_num;i++){  
        //Ϊÿ�����л���������bids����û�г��ֿ��л����ˣ��Ȳ��ܣ�
        if(robot_info_table[i].task_status == -1){
            if(!flag){
                updateAvailList();   //��ʼʱӦ��ֻ��1��2��3��������̨��Ӧ������
                flag = true;
            }
            generateBids(i);
        }
    }
    assignTaskfromBids();
    //end
    
}

//���ѡ��
void outputDecision(){
    for(int i=0;i<decision_num;i++){
        printf("%s %d", decision_string[decision_info_table[i].type], decision_info_table[i].robot);
        if(decision_info_table[i].type<=1) printf(" %f\n", decision_info_table[i].param);
        else printf("\n");
    }
}

//����������price�����񲢷���
void assignTaskfromBids(){
    //ǰ�ᱣ֤�������е�robot��Ӧ��bids_listΪ��
    float max_price;
    int max_robot_id;
    while(1){
        max_price = 0;
        max_robot_id = -1;
        //bids_list��¶����
        for(int i=0;i<robot_num;i++){
            if(!bids_list[i].empty()){
                while(isTaskBusy(bids_list[i][0].task_id)){
                    bids_list[i].erase(bids_list[i].begin());
                }
                if(max_price<bids_list[i][0].price){
                    max_price = bids_list[i][0].price;
                    max_robot_id = i;
                }
            }
        }
        if(max_robot_id != -1){
            //�������������ɹ���robot���������bids_list
            addTasktoRobot(max_robot_id,bids_list[max_robot_id][0].task_id,0);
            tagBusyTask(bids_list[max_robot_id][0].task_id);
            clearBidList(max_robot_id);
        }else{
            break;
        }    
    }
}

//�������������ҳ���ǰ��������id�ŵ�avail_taskid_list
void updateAvailList(void){
    clearTaskIdList();  //�������������������
    for(int i=0;i<task_num;i++){
        int s = getSourceOfTask(i);
        int d = getDestOfTask(i);
        //source�����Ѿ����˲�Ʒ ���� ����������
        if(getTimeOfStation(s)==-1 && getOkOfStation(s)==0){
            continue;
        }
        //dest������λ�÷Ų�Ʒ ��ʱ������dest�������������
        int type = getTypeOfStation(s);
        int raw = getRawOfStation(d);
        if(((1<<(type-1)) & raw) == 0){
            continue;
        }
        //��ǰid����
        addTaskId(i);
    }
}

//Ϊrobot_id���ɱ��۲������ݶ��ı���=�����˵������ڵ�ľ���+���/����̨֮��ľ���
void generateBids(int robot_id){
    clearBidList(robot_id);
    //��avail_taskid_list��¶
    for(int task_id: avail_taskid_list){
        float xx = robot_info_table[robot_id].x-station_info_table[waiting_task_list[task_id].source].x;
        float yy = robot_info_table[robot_id].y-station_info_table[waiting_task_list[task_id].source].y;
        //������Ҫ��һ����ȷ�� TODO
        addBidInfo(robot_id,task_id,sqrt(xx+yy)+getWeightOfTask(task_id));
    }
    sortBidList(robot_id);
}

//��ͼ��ʼ����������������
//��ʼ������е�Ȩ�� = ���/���صľ���
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
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 2:
            {
                if(station_info_table[j].type==4 || station_info_table[j].type==6 || station_info_table[j].type==9){
                    int value=3200;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 3:
            {
                if(station_info_table[j].type==6 || station_info_table[j].type==5 || station_info_table[j].type==9){
                    int value=3400;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 4:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7100;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 5:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=7800;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 6:
            {
                if(station_info_table[j].type==7 || station_info_table[j].type==9){
                    int value=8300;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            case 7:
            {
                if(station_info_table[j].type==8 || station_info_table[j].type==9){
                    int value=29000;
                    float distance = cacuStationDis(i,j);
                    addTaskInfo(i,j,value,distance,value/distance);
                    addStationTask(i,j,task_num);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}
