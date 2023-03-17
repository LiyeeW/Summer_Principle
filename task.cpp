#include "task.h"

//任务队列，机器人取走任务后标记为busy
vector<TaskInfo> waiting_task_list;

//能被执行的任务队列
vector<int> avail_taskid_list;

//最大任务数量
int task_num;

//新增一个任务，不用排序
void addTaskInfo(int s, int d, int value, float distance, float weight){
    waiting_task_list.push_back({task_num,s,d,value,distance,weight,false});
    task_num++;
}

bool taskcompare(TaskInfo t1, TaskInfo t2){
    return t1.weight>t2.weight;
}

//所有任务按照优先级排序
void sortTaskList(void){
    sort(waiting_task_list.begin(),waiting_task_list.end(),taskcompare);
}

//查看任务是否已经在被处理了
bool isTaskBusy(int id){
    return waiting_task_list[id].busy;
}

//标记任务忙碌
void tagBusyTask(int id){
    waiting_task_list[id].busy = true;
}

//标记任务空闲
void tagFreeTask(int id){
    waiting_task_list[id].busy = false;
}

//获取source
int getSourceOfTask(int id){
    return waiting_task_list[id].source;
}

//获取dest
int getDestOfTask(int id){
    return waiting_task_list[id].dest;
}

//将能执行的任务id加入taskid_list
void addTaskId(int id){
    avail_taskid_list.push_back(id);
}

//清空taskId_list
void clearTaskIdList(void){
    avail_taskid_list.clear();
}

//获取任务固定权重
float getWeightOfTask(int id){
    return waiting_task_list[id].weight;
}