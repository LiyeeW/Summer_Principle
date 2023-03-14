#include "task.h"

//任务队列
vector<TaskInfo> waiting_task_list;

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