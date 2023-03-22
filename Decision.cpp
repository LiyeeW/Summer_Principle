#include "Decision.h"
#include "Robot.h"
#include "Station.h"
#include "Score.h"
#include "task.h"
#include "Bid.h"
#include "Movement.h"

#include <cstring>
#include <iostream>

using namespace std;

//做出选择
void makeDecision(){
    //如果出现空闲机器人：
    //0. （如果有任务完成）标记已完成的任务为free
    //1. 生成能获取的任务id列表
    //2. 为空闲机器人生成bids
    //3. 为空闲机器人分配task
    
    //将已完成的任务状态更新
    for(int i=0;i<robot_num;i++){
        if(robot_info_table[i].task_status == 2){
            //当有机器人robot_id完成任务后，更新任务状态
            tagFreeTask(getTaskofRobot(i));
            //标记机器人状态空闲
            //addTasktoRobot(i,-1,-1);
            setTaskStatusofRobot(i,-1);     //上一个任务id仍保存着，后面使用
        }
    }
    bool flag = false;
    for(int i=0;i<robot_num;i++){  
        //为每个空闲机器人生成bids：（没有出现空闲机器人：先不管）
        if(robot_info_table[i].task_status == -1){
            if(!flag){
                updateAvailList();   //初始时应该只有1，2，3生产工作台对应的任务
                flag = true;
            }
            generateBids(i);
        }
    }
    //cerr<<"assign now"<<endl;
    assignTaskfromBids();
    //end
    
}

//缓解碰撞，不能三个以上的robot前往同一个station
bool isTaskStationRepeat(int taskid){
    int s = getSourceOfTask(taskid);
    int d = getDestOfTask(taskid);
    int s_num=0, d_num=0;
    for(int i=0;i<robot_num;i++){
        if(robot_info_table[i].task_id != -1 && robot_info_table[i].task_status != -1){
            if(getSourceOfTask(robot_info_table[i].task_id) == s)   s_num++;
            if(getDestOfTask(robot_info_table[i].task_id) == d)     d_num++;
        }
    }
    if(s_num>=2 || d_num>=2){
        return true;
    }else{
        return false;
    }
}

//判断当前任务是否冲突，即不能两个robot前往同一个source取货，或前往同一个dest卖货
//有可能发生任务列表全部冲突，无任务分配而报错的可能
//所以放宽冲突条件，取消dest冲突
bool isTaskConflict(int taskid){
    int s = getSourceOfTask(taskid);
    int d = getDestOfTask(taskid);
    //判断冲突
    for(int i=0;i<robot_num;i++){
        //当前处于任务执行状态
        if(robot_info_table[i].task_id != -1 && robot_info_table[i].task_status != -1){
            int another_s = getSourceOfTask(robot_info_table[i].task_id);
            //cerr<<"robot "<<i<<" "<<another_s<<" "<<robot_info_table[i].task_status<<getTimeOfStation(s)<<endl;
            //不能有另一个robot前往同一个s，且s并未处于阻塞状态
            if(robot_info_table[i].task_status == 0 && s == another_s && getTimeOfStation(s)!=0) return true;   //source冲突

            //不能有另一个robot前往同一个d送相同的货，且d并非处于即将能生产的状态
            int d_type = station_info_table[d].type, s_type = station_info_table[s].type;
            int now_raw = station_info_table[d].raw + (1<<s_type);
            int flag = false;
            switch(d_type){  //4~7
                case 4:{
                    flag = now_raw == ((1<<1)+(1<<2));  //为true表示原材料格即将满
                    break;
                }
                case 5:{
                    flag = now_raw == ((1<<1)+(1<<3));
                    break;
                }
                case 6:{
                    flag = now_raw == ((1<<2)+(1<<3));
                    break;
                }
                case 7:{
                    flag = now_raw == ((1<<4)+(1<<5)+(1<<6));
                    break;
                }
                default:
                    break;
            }
            flag = flag && (getTimeOfStation(d)==-1);   //true：能立马消耗原材料格
            if(d == getDestOfTask(robot_info_table[i].task_id) && station_info_table[d].type<8
                && getTypeOfStation(s)==getTypeOfStation(another_s) && !flag) return true;  //dest冲突
        }
    }
    return false;
}

//计算具有最大price的任务并返回
void assignTaskfromBids(){
    //前提保证：不空闲的robot对应的bids_list为空
    float max_price;
    int max_robot_id;
    while(1){
        max_price = 0;
        max_robot_id = -1;
        //bids_list暴露出来
        for(int i=0;i<robot_num;i++){
            //cerr<<"robot "<<i<<" "<<bids_list[i].size()<<endl;
            if(!bids_list[i].empty()){
                //不能分配相同的任务给两个robot，此外，不能两个robot前往同一个source取货，或前往同一个dest卖货
                while(isTaskBusy(bids_list[i][0].task_id) || isTaskConflict(bids_list[i][0].task_id)){
                    bids_list[i].erase(bids_list[i].begin());
                    if(bids_list[i].empty()){
                        cerr<<current_frame<<": no task assign for robot "<<i<<endl;
                    }
                }
                if(max_price<bids_list[i][0].price){
                    max_price = bids_list[i][0].price;
                    max_robot_id = i;
                }
            }
        }
        if(max_robot_id != -1){
            //分配任务给竞标成功的robot，并清空其bids_list
            addTasktoRobot(max_robot_id,bids_list[max_robot_id][0].task_id,0);
            //debug
            int taskid = bids_list[max_robot_id][0].task_id;
            int s = waiting_task_list[taskid].source, d = waiting_task_list[taskid].dest;
            std::cerr<<"assign task to robot "<<max_robot_id<<",price="<<max_price<<",s is "<<s<<",d is "<<d<<":  ";
            cerr<<"     s: ("<<station_info_table[s].x<<", "<<station_info_table[s].y<<"), type="<<station_info_table[s].type;
            cerr<<"     d: ("<<station_info_table[d].x<<", "<<station_info_table[d].y<<"), type="<<station_info_table[d].type<<endl;
            //
            tagBusyTask(bids_list[max_robot_id][0].task_id);
            clearBidList(max_robot_id);
        }else{
            break;
        }    
    }
}

//遍历所有任务，找出能前往的任务id放到avail_taskid_list
void updateAvailList(void){
    clearTaskIdList();  //先清空能做的所有任务
    for(int i=0;i<task_num;i++){
        int s = getSourceOfTask(i);
        int d = getDestOfTask(i);
        //source必须已经有了产品 或者 处于生产中
        //source处于生产的时间不能过长
        //需要考虑到初始状态下每个staion的ok=0，且status=-1
        if((getTimeOfStation(s)==-1 || getTimeOfStation(s)>200) && getOkOfStation(s)==0){        //ok=0的情况下不可能出现time=0
            continue;
        }
        //dest必须有位置放产品
        //如果dest即将消耗现在的原材料进入生产，也考虑进来
        int type = getTypeOfStation(s);
        int raw = getRawOfStation(d);
        int flag = isStaionFullRow(d);   //为true表示原材料格已满
        //当前产品格为空，原材料格已满，说明正在进行生产
        //或是当前有对应的空闲原材料格
        if((flag && getOkOfStation(d)==0) || ((1<<type) & raw) == 0){   
            //当前id加入
            addTaskId(i);
        }        
    }
    if(isEmptyAvaiList()){
        cerr<<"---------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!bug:no availtasklist"<<endl;
    }
}

//为robot_id生成报价并排序，暂定的报价=差价/(机器人到生产节点的距离+工作台之间的距离)
//availtask_list是所有符合条件的task集合，包括source正在生产 or dest原材料格已满且生产线正在被占用
//需要针对robot所在的位置进一步报价优化，不能等待source太久，source生产时间-到达source时间，dest生产时间-s->d时间-max(source生产，到source)
void generateBids(int robot_id){
    clearBidList(robot_id);
    //将avail_taskid_list暴露
    for(int task_id: avail_taskid_list){
        float xx = robot_info_table[robot_id].x-station_info_table[waiting_task_list[task_id].source].x;
        float yy = robot_info_table[robot_id].y-station_info_table[waiting_task_list[task_id].source].y;
        //报价需要进一步精确化 TODO
        //source工作台的产品格阻塞，增添对应任务的权重
        int s = getSourceOfTask(task_id), d=getDestOfTask(task_id);
        
        float extra_w_source = 0, extra_w_dest = 0;
        if(getTimeOfStation(s) == 0 && getTypeOfStation(s)>3){
            extra_w_source = 1.5;
        }
        //dest工作台就差这一个产品就进入生产状态，增加权重 TODO
        int d_type = station_info_table[d].type, s_type = station_info_table[s].type;
        int now_raw = station_info_table[d].raw + (1<<s_type);
        int flag = false;
        switch(d_type){  //4~7
            case 4:{
                flag = now_raw == ((1<<1)+(1<<2));  //为true表示原材料格即将满
                break;
            }
            case 5:{
                flag = now_raw == ((1<<1)+(1<<3));
                break;
            }
            case 6:{
                flag = now_raw == ((1<<2)+(1<<3));
                break;
            }
            case 7:{
                flag = now_raw == ((1<<4)+(1<<5)+(1<<6));
                break;
            }
            default:
                break;
        }
        flag = flag && (getOkOfStation(d)==0);   //true：能立马消耗原材料格，且此时产品格为空
        if(flag){
            extra_w_dest = 1.0;
        }
        
        ////////////////////////考虑等待时间
        int robot_to_source = getStationsFrameCost(getDestOfTask(robot_info_table[robot_id].task_id),s);
        int source_to_dest = getStationsFrameCost(s,d,getDestOfTask(robot_info_table[robot_id].task_id));
        int source_wait = 0;
        //如果source正在生产 且 目前产品格为空
        if(getOkOfStation(s)==0 && getTimeOfStation(s)>0){
            source_wait = max(0,getTimeOfStation(s)-robot_to_source);
        }
        //如果dest目前原材料格已满 且 目前产品格为空
        int dest_wait = 0;
        int isflag = isStaionFullRow(d);   //为true表示原材料格已满
        if(getOkOfStation(d)==0 && isflag){
            dest_wait = max(0,getTimeOfStation(d)-robot_to_source-source_to_dest-source_wait);
        }
        //////////////////////
        //addBidInfo(robot_id,task_id,extra_w_source+extra_w_dest+waiting_task_list[task_id].value/sqrt(xx*xx+100*yy*yy));  //300
        //addBidInfo(robot_id,task_id,extra_w_source+extra_w_dest+waiting_task_list[task_id].value/100/(sqrt(xx*xx+4*yy*yy)+1.5*waiting_task_list[task_id].distance));
        addBidInfo(robot_id,task_id,extra_w_source+waiting_task_list[task_id].value/100/(sqrt(xx*xx+yy*yy)+1.5*waiting_task_list[task_id].distance)-dest_wait/200-source_wait/200);
    }
    sortBidList(robot_id);
}

//地图初始化：生成所有任务
//初始任务队列的权重 = 差价/两地的距离
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
    std::cerr<<"initmap: total "<<task_num<<" tasks"<<endl;
    cerr<<"total robotnum: "<<robot_num<<" robots"<<endl;
}
